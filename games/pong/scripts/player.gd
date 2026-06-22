extends CharacterBody2D

const SPEED = 300.0

func _physics_process(delta: float) -> void:
	var dir := Input.get_axis("ui_up", "ui_down")
	velocity.y = dir * SPEED
	move_and_slide()
