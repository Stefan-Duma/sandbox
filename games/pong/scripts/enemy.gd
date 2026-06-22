extends CharacterBody2D

@export var ball : CharacterBody2D
@onready var start_y: float = position.y

const MAX_SPEED := 200
const KP = 10.0 # Proportional gain
func _physics_process(delta: float) -> void:
	if not ball:
		return
		
	var target_y = start_y
	if ball.direction.x < 0:
		target_y = ball.position.y
	
	var distance = target_y - position.y
	var desired_velocity = distance * KP
	
	velocity = Vector2(0, clamp(desired_velocity, -MAX_SPEED, MAX_SPEED))
	
	move_and_slide()
