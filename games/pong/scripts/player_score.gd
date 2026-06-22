extends Label

func _on_ball_update_score(player_score: Variant, enemy_score: Variant) -> void:
	text = str(player_score)
