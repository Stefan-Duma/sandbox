extends CharacterBody2D

var speed := 400
var direction := Vector2.ZERO
var player_score := 0
var enemy_score := 0

signal update_score(player_score, enemy_score)

# Prevents vertical angles
func limit_vertical_bounce(dir: Vector2) -> Vector2:
	var angle = dir.angle()
	var max_angle = deg_to_rad(60)
	
	if dir.y > 0 and (angle > max_angle and angle < PI - max_angle):
		var safe_angle = max_angle if dir.x > 0 else PI - max_angle
		dir = Vector2.RIGHT.rotated(safe_angle)
	
	elif dir.y < 0 and (angle < -max_angle and angle > -PI + max_angle):
		var safe_angle = -max_angle if dir.x > 0 else -PI + max_angle
		dir = Vector2.RIGHT.rotated(safe_angle)
		
	return dir.normalized()

func reset() -> void:
	position = Vector2.ZERO
	direction = get_start_direction()
	speed = 400

func get_start_direction() -> Vector2:
	var x_dir = [-1.0, 1.0].pick_random()
	var y_dir = randf_range(-0.6, 0.6)
	
	return Vector2(x_dir, y_dir).normalized()

func _init() -> void:
	direction = get_start_direction()

func _physics_process(delta: float) -> void:
	var collision = move_and_collide(direction * speed * delta)
	if collision:
		direction = direction.bounce(collision.get_normal())
		direction = limit_vertical_bounce(direction)
		speed += 15

func _on_left_body_entered(body: Node2D) -> void:
	reset()
	player_score += 1
	update_score.emit(player_score, enemy_score)

func _on_right_body_entered(body: Node2D) -> void:
	reset()
	enemy_score += 1
	update_score.emit(player_score, enemy_score)
