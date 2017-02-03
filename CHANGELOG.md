## Current branch

- [Deprecation] removed method `get_goal_speed_angle` that only duplicated `get_moving_speed` and brought nothing
- [API change] renamed methods `reg_goal_speed_angle` and `set_goal_speed_angle` respectively to `reg_moving_speed_angle` and `set_moving_speed_angle`
- [Improvement] add information on the unit in error message for position and speed limit overflow
- [API change] add protocol specific methods to set the torque limit (called goal torque for Protocol2)