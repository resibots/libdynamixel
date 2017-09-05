## Current branch

- [Deprecation] removed method `get_goal_speed_angle` that only duplicated `get_moving_speed` and brought nothing
- [API change] renamed methods `reg_goal_speed_angle` and `set_goal_speed_angle` respectively to `reg_moving_speed_angle` and `set_moving_speed_angle`
- [Improvement] add information on the unit in error message for position and speed limit overflow
- [Command line] allow to set timeout for the scan for connected servos
- [Command line] catch exceptions raised by boost::program_options when improper options are used