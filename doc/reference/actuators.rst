.. _supported_actuators_and_fields:

Supported actuators and their control tables
============================================

Table of supported servos
-------------------------

The links in this table point to the fields specific to each model. All these servo have in comon the fields in :ref:`next section <common_fields>`

+--------+------------------+------------------------------------------------------------------------------------------+
| Series | Protocol version | Models                                                                                   |
+========+==================+==========================================================================================+
| AX     | 1                | :ref:`AX12, AX12w, AX18 <ax_series>`                                                     |
+--------+------------------+------------------------------------------------------------------------------------------+
| MX     | 1                | :ref:`MX12, MX28 <mx_series>`, :ref:`MX64`, :ref:`MX106`                                 |
+--------+------------------+------------------------------------------------------------------------------------------+
| EX     | 1                | :ref:`EX106`                                                                             |
+--------+------------------+------------------------------------------------------------------------------------------+
| XL     | 2                | :ref:`XL320`                                                                             |
+--------+------------------+------------------------------------------------------------------------------------------+
| Pro L  | 2                | :ref:`L-42-10-S300, L-54-30-S400, L-54-30-S500, L-54-50-S290, L-54-50-S500 <pro_series>` |
+--------+------------------+------------------------------------------------------------------------------------------+
| Pro M  | 2                | :ref:`M-42-10-S260, M-54-40-S250, M-54-60-S250 <pro_series>`                             |
+--------+------------------+------------------------------------------------------------------------------------------+
| Pro H  | 2                | :ref:`H-42-20-S300, H-54-100-S500, H-54-200-S500 <pro_series>`                           |
+--------+------------------+------------------------------------------------------------------------------------------+

.. _common_fields:

Field names common to all servos
--------------------------------
For the meaning of the fields, please refer to Robotis' `official documentation <http://support.robotis.com/en/>`__.

Read-only fields:

* ``model_number``
* ``firmware_version``
* ``present_position``
* ``present_speed``
* ``present_voltage``
* ``present_temperature``
* ``registered``
* ``moving``

Read-write fields:

* ``id``
* ``baudrate``
* ``return_delay_time``
* ``highest_temperature_limit``
* ``highest_voltage_limit``
* ``lowest_voltage_limit``
* ``status_return_level``
* ``alarm_shutdown``
* ``torque_enable``
* ``goal_position``
* ``moving_speed``

.. _ax_series:

AX series
---------

Supported models are

* **AX12**,
* **AX12w** and
* **AX18**.

Read-only field:

* ``present_load``

Read-write fields:

* ``cw_angle_limit``
* ``ccw_angle_limit``
* ``max_torque``
* ``alarm_led``
* ``led``
* ``cw_compliance_margin``
* ``ccw_compliance_margin``
* ``cw_compliance_slope``
* ``ccw_compliance_slope``
* ``torque_limit``
* ``lock``
* ``punch``

.. _mx_series:

MX series
---------
Common  to all MX servos, including **MX12** and **MX28**.

Read-only field:

* ``present_load``

Read-write fields:

* ``cw_angle_limit``
* ``ccw_angle_limit``
* ``max_torque``
* ``alarm_led``
* ``multi_turn_offset``
* ``resolution_divider``
* ``led``
* ``d_gain``
* ``i_gain``
* ``p_gain``
* ``torque_limit``
* ``lock``
* ``punch``
* ``goal_acceleration``

.. _MX64:

MX64
^^^^
Read-only field:

* ``current``

Read-write fields:

* ``torque_control_mode_enabled``
* ``goal_torque``

.. _MX106:

MX106
^^^^^
Read-only field:

* ``current``

Read-write fields:

* ``torque_control_mode_enabled``
* ``goal_torque``
* ``drive_mode``

.. _EX106:

EX106
-----
Read-only fields:

* ``present_load``
* ``sensed_current``

Read-write fields:

* ``cw_angle_limit``
* ``ccw_angle_limit``
* ``drive_mode``
* ``max_torque``
* ``alarm_led``
* ``led``
* ``cw_compliance_margin``
* ``ccw_compliance_margin``
* ``cw_compliance_slope``
* ``ccw_compliance_slope``
* ``torque_limit``
* ``lock``
* ``punch``

.. _XL320:

XL320
-----
Read-only fields:

* ``hardware_error_status``
* ``present_load``

Read-write fields:

* ``cw_angle_limit``
* ``ccw_angle_limit``
* ``control_mode``
* ``max_torque``
* ``led``
* ``d_gain``
* ``i_gain``
* ``p_gain``
* ``goal_torque``
* ``punch``

.. _pro_series:

Pro series
----------

.. caution:: Altough the field names are the same for the L42 and other servos, their meanings still differ with the other Dynamixel Pros. Please check Robotis' official documentation.

Supported models are

* **Pro H-42-20-S300**,
* **Pro M-54-60-S250**,
* **Pro M-54-40-S250**,
* **Pro M-42-10-S260**,
* **Pro L-54-50-S500**,
* **Pro L-54-50-S290**,
* **Pro L-54-30-S500**,
* **Pro L-54-30-S400**,
* **Pro L-42-10-S300**,
* **Pro H-54-200-S500** and
* **Pro H-54-100-S500**.

Read-only fields:

* ``current``
* ``hardware_error_status``

Read-write fields:

* ``model_info``
* ``operating_mode``
* ``homing_offset``
* ``moving_threshold``
* ``acceleration_limit``
* ``torque_limit``
* ``speed_limit``
* ``max_position_limit``
* ``min_position_limit``
* ``led_r``
* ``led_g``
* ``led_b``
* ``velocity_i_gain``
* ``velocity_p_gain``
* ``position_p_gain``
* ``goal_torque``
* ``goal_acceleration``