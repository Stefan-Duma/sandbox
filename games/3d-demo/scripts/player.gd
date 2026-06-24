extends CharacterBody3D

const PUSH_FORCE = 1
const SPEED = 5.0
const GRAVITY_PULL = 9.8

# --- BOBBING CONFIGURATION ---
const BOB_FREQUENCY = 2.4
const BOB_AMPLITUDE_Y = 0.03   # Vertical displacement (Up/Down)
const BOB_AMPLITUDE_X = 0.015  # Horizontal sway amplitude (Left/Right)

var bob_time = 0.0

# --- MOVEMENT VARS ---
const WALK_SPEED = 5.0
const SPRINT_SPEED = 9.0
const JUMP_VELOCITY = 4.5

# --- TIME CONTROL VARS ---
const NORMAL_TIME = 1.0
const SLOW_TIME = 0.01


@onready var default_blade_x: float = %Camera3D/Blade.position.x
@onready var default_blade_y: float = %Camera3D/Blade.position.y
@onready var gravity_ray: RayCast3D = $RayCast3D

func _ready() -> void:
	Input.mouse_mode = Input.MOUSE_MODE_CAPTURED

func _unhandled_input(event: InputEvent) -> void:
	if event is InputEventMouseMotion:
		rotation_degrees.y -= event.relative.x * 0.5
		%Camera3D.rotation_degrees.x -= event.relative.y
		%Camera3D.rotation_degrees.x = clamp(%Camera3D.rotation_degrees.x, -60, 60)
	#elif event.is_action_pressed('ui_cancel'):
		#Input.mouse_mode = Input.MOUSE_MODE_VISIBLE

func _physics_process(delta: float) -> void:
	var surface_normal = Vector3.UP 
	
	if gravity_ray.is_colliding():
		surface_normal = gravity_ray.get_collision_normal()
	elif is_on_wall():
		surface_normal = get_wall_normal()
	elif is_on_floor():
		surface_normal = get_floor_normal()
		
	up_direction = surface_normal
	
	if not is_on_floor():
		velocity -= surface_normal * GRAVITY_PULL * delta
	
	var is_sprinting = Input.is_action_pressed("sprint")
	
	var target_time = SLOW_TIME if is_sprinting else NORMAL_TIME
	Engine.time_scale = lerp(Engine.time_scale, target_time, 0.1)
	
	var base_speed = SPRINT_SPEED if is_sprinting else WALK_SPEED
	var current_speed = base_speed / Engine.time_scale
	
	if Input.is_action_just_pressed("ui_accept") and is_on_floor():
		velocity += surface_normal * JUMP_VELOCITY

	var input_dir := Input.get_vector("ui_left", "ui_right", "ui_up", "ui_down")
	var direction := (transform.basis * Vector3(input_dir.x, 0, input_dir.y)).normalized()
	
	var vertical_velocity = surface_normal * velocity.dot(surface_normal)
	
	if direction:
		velocity = vertical_velocity + (direction * current_speed)
	else:
		velocity = velocity.move_toward(vertical_velocity, current_speed)
	
	move_and_slide()
	align_to_surface(surface_normal, delta)
	handle_blade_bob(delta)
	handle_camera_fov(is_sprinting)
	handle_physics_push()

func handle_blade_bob(delta: float) -> void:
	if is_on_floor() and (abs(velocity.x) > 0.1 or abs(velocity.z) > 0.1):
		bob_time += delta * SPEED * BOB_FREQUENCY
		
		var target_y = default_blade_y + sin(bob_time) * BOB_AMPLITUDE_Y
		var target_x = default_blade_x + cos(bob_time * 0.5) * BOB_AMPLITUDE_X
		
		%Camera3D/Blade.position.y = target_y
		%Camera3D/Blade.position.x = target_x
		
	else:
		bob_time = 0.0
		%Camera3D/Blade.position.y = move_toward(%Camera3D/Blade.position.y, default_blade_y, delta * 0.2)
		%Camera3D/Blade.position.x = move_toward(%Camera3D/Blade.position.x, default_blade_x, delta * 0.2)

func handle_camera_fov(is_sprinting: bool) -> void:
	var target_fov = 95.0 if is_sprinting else 75.0
	
	%Camera3D.fov = lerp(%Camera3D.fov, target_fov, 0.1)

func handle_physics_push() -> void:
	for i in get_slide_collision_count():
		var collision = get_slide_collision(i)
		var collider = collision.get_collider()
		
		if collider is RigidBody3D:
			var push_dir = -collision.get_normal()
			push_dir.y = 0 
			collider.apply_central_impulse(push_dir.normalized() * PUSH_FORCE)

func align_to_surface(surface_normal: Vector3, delta: float) -> void:
	var current_basis = global_transform.basis
	
	if current_basis.y.is_equal_approx(surface_normal):
		return
	
	var target_rotation = Basis(Quaternion(current_basis.y, surface_normal)) * current_basis
	
	var immune_delta = delta / Engine.time_scale
	global_transform.basis = current_basis.slerp(target_rotation, immune_delta * 8.0).orthonormalized()
