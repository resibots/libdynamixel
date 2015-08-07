#ifndef DYNAMIXEL_AX12_HPP_
#define DYNAMIXEL_AX12_HPP_

#include "misc.hpp"
#include "instruction.hpp"
namespace dynamixel
{
  namespace ax12
  {
     // constants
    static const unsigned int model_number = 0x000c;
    static const unsigned int goal_position_min = 0x0000;
    static const unsigned int goal_position_max = 0x03ff;
    /// Asks the servo-motor to move with its maximal speed capacity.
    /// Works only in joint mode
    static const unsigned int speed_uncontroled = 0x0000;
    static const unsigned int speed_min = 0x0001;
    static const unsigned int speed_max = 0x03ff;
    static const unsigned int ccw_turn_direction = 0x0000;
    static const unsigned int cw_turn_direction = 0x0400;
    static const unsigned int max_speed_in_rpm = 114;

     // action
    namespace action
    {
      static const byte_t ping = 0x01; // 0 params
      static const byte_t read_data = 0x02; // 2 params
      static const byte_t write_data = 0x03; // 2~ params
      static const byte_t reg_write = 0x04; // 2~params
      static const byte_t action = 0x05; // 0 params
      static const byte_t reset = 0x06; // 0;
      static const byte_t sync_write = 0x83; // 4~
    }

     // control table
    namespace ctrl
    {
      static const byte_t model_number_low = 0x00;
      static const byte_t model_number_hi = 0x01;
      static const byte_t firmware_version = 0x02;
      static const byte_t id = 0x03;
      static const byte_t baudrate = 0x04;
      static const byte_t return_delay_time = 0x05;
      static const byte_t cw_angle_limit_lo = 0x06;
      static const byte_t cw_angle_limit_hi = 0x07;
      static const byte_t ccw_angle_limit_lo = 0x08;
      static const byte_t ccw_angle_limit_hi = 0x09;
      static const byte_t highest_limit_temperature = 0x0b;
      static const byte_t lowest_limit_voltage = 0x0c;
      static const byte_t highest_limit_voltage = 0x0d;
      static const byte_t max_torque_lo = 0x0e;
      static const byte_t max_torque_hi = 0x0f;
      static const byte_t status_return_level = 0x10;
      static const byte_t alarm_led = 0x11;
      static const byte_t alarm_shutdown = 0x12;
      static const byte_t down_calibration_lo = 0x14;
      static const byte_t down_calibration_hi = 0x15;
      static const byte_t up_calibration_lo = 0x16;
      static const byte_t up_calibration_hi = 0x17;
      static const byte_t torque_enable = 0x18;
      static const byte_t led = 0x19;
      static const byte_t cw_compliance_margin = 0x1a;
      static const byte_t ccw_compliance_margin = 0x1b;
      static const byte_t cw_compliance_slope = 0x1c;
      static const byte_t ccw_compliance_slope = 0x1d;
      static const byte_t goal_position_lo = 0x1e;
      static const byte_t goal_position_hi = 0x1f;
      static const byte_t moving_speed_lo = 0x20;
      static const byte_t moving_speed_hi = 0x21;
      static const byte_t torque_limit_lo = 0x22;
      static const byte_t torque_limit_hi = 0x23;
      static const byte_t present_pos_lo = 0x24;
      static const byte_t present_pos_hi = 0x25;
      static const byte_t present_speed_lo = 0x26;
      static const byte_t present_speed_hi = 0x27;
      static const byte_t present_load_lo = 0x28;
      static const byte_t present_load_hi = 0x29;
      static const byte_t present_voltage = 0x2a;
      static const byte_t present_temperature = 0x2b;
      static const byte_t registered_instruction = 0x2c;
      static const byte_t moving = 0x2e;
      static const byte_t lock = 0x2f;
      static const byte_t punch_lo = 0x30;
      static const byte_t punch_hi = 0x31;
    }


     // \{ low-level AX12 instructions
    struct Ping : public Instruction
    {
      Ping(byte_t id) : Instruction(id, action::ping) {
      }
    };

    struct ReadData : public Instruction
    {
      ReadData(byte_t id,
               byte_t p1,
               byte_t p2) :
        Instruction(id, action::read_data, p1, p2) {
      }
    };

    struct WriteData : public Instruction
    {
      WriteData(byte_t id,
                const std::vector<byte_t>&params) :
        Instruction(id, action::write_data, params)
      {
        assert(params.size() >= 2);
      }

      WriteData(byte_t id,
                byte_t p1,
                byte_t p2) :
        Instruction(id, action::write_data, p1, p2) {
      }

      WriteData(byte_t id,
                byte_t p1,
                byte_t p2,
                byte_t p3) :
        Instruction(id, action::write_data, p1, p2, p3) {
      }
    };

    struct RegWrite : public Instruction
    {
      RegWrite(byte_t id,
               const std::vector<byte_t>&params) :
        Instruction(id, action::reg_write, params)
      {
        assert(params.size() >= 2);
      }
    };

    struct Action : public Instruction
    {
      Action(byte_t id) : Instruction(id, action::action) {
      }
    };

    struct Reset : public Instruction
    {
      Reset(byte_t id) : Instruction(id, action::reset) {
      }
    };

    struct SyncWrite : public Instruction
    {
      SyncWrite(byte_t id,
                const std::vector<byte_t>&params) :
        Instruction(id, action::sync_write, params)
      {
        assert(params.size() >= 4);
      }
    };

    class SetPosition : public Instruction
    {
      public:
        SetPosition(const byte_t& id, const int position) : Instruction(id,action::sync_write)
        {
          std::vector<byte_t> params;
          params.push_back(ax12::ctrl::goal_position_lo);
          params.push_back(2);
          params.push_back(id);
          params.push_back(position & 0x00FF);
          params.push_back((position & 0xFF00) >> 8);
          rebuild_packet(params);
        }
    };

     // -- useful high-level instructions --
     // set position of all pointed ax12
    class SetPositions : public Instruction
    {
      public:
        SetPositions(const std::vector<byte_t> &ids,
                     const std::vector<int> positions) :
        Instruction(broadcast, action::sync_write), _ids(ids)
        {
          _set_positions(_ids, positions);
        }

        void set_positions(const std::vector<int> positions)
        {
          _set_positions(_ids, positions);
        }

      protected:
        std::vector<byte_t> _ids;

        void _set_positions(const std::vector<byte_t>& ids,
                            const std::vector<int> positions)
        {
          assert(positions.size() == _ids.size());
          std::vector<byte_t> params;
          params.push_back(ax12::ctrl::goal_position_lo);
          params.push_back(2);
          for (size_t i = 0; i < ids.size(); ++i)
          {
         //   CHECK(positions[i] < 1023, "SetPosition: position > 1023");
            params.push_back(ids[i]);
            params.push_back(positions[i] & 0x00FF);
            params.push_back((positions[i] & 0xFF00) >> 8);
          }
          rebuild_packet(params);
        }
    };

    class SetSpeed : public Instruction
    {
      public:
        SetSpeed(const byte_t& id, const int speed, bool direction=false) : Instruction(id,action::sync_write)
        {
          std::vector<byte_t> params;
          params.push_back(ax12::ctrl::moving_speed_lo);
          params.push_back(2);
          CHECK(speed <= 1023, "SetSpeed: speed > 1023");
          params.push_back(id);
          params.push_back(speed & 0x00FF);
          int s = (speed | (direction << 10));
          params.push_back(s >> 8);
          rebuild_packet(params);
        }
    };

    class SetSpeeds : public Instruction
    {
      public:
        SetSpeeds(const std::vector<byte_t> &ids,
                  const std::vector<int> speeds,
                  const std::vector<bool>& directions) :
        Instruction(broadcast, action::sync_write),
        _ids(ids)
        {
          _set_speeds(speeds, directions);
        }

        SetSpeeds(const std::vector<byte_t> &ids,
                  const std::vector<int> speeds) :
        Instruction(broadcast, action::sync_write),
        _ids(ids)
        {
          _set_speeds(speeds);
        }

        void set_speeds(const std::vector<int> speeds,
                        const std::vector<bool>& directions)
        {
          _set_speeds(speeds, directions);
        }

        void set_speeds(const std::vector<int> speeds)
        {
          _set_speeds(speeds);
        }

      protected:
        std::vector<byte_t> _ids;

        void _set_speeds(const std::vector<int> speeds,
                         std::vector<bool> directions)
        {
          assert(_ids.size() == directions.size());
          assert(speeds.size() == directions.size());
          std::vector<byte_t> params;
          params.push_back(ax12::ctrl::moving_speed_lo);
          params.push_back(2);
          for (size_t i = 0; i < _ids.size(); ++i)
          {
            CHECK(speeds[i] <= 1023, "SetSpeed: speed > 1023");
            params.push_back(_ids[i]);
            params.push_back(speeds[i] & 0x00FF);
            int s = (speeds[i] | (directions[i] << 10));
            params.push_back(s >> 8);
          }
          rebuild_packet(params);
        }

        void _set_speeds(const std::vector<int> speeds)
        {
          assert(_ids.size() == speeds.size());
          std::vector<byte_t> params;
          params.push_back(ax12::ctrl::moving_speed_lo);
          params.push_back(2);
          for (size_t i = 0; i < _ids.size(); ++i)
          {
            CHECK(speeds[i] <= 1023, "SetSpeed: speed > 1023");
            params.push_back(_ids[i]);
            params.push_back(speeds[i] & 0x00FF);
            params.push_back(speeds[i] >> 8);
          }
          rebuild_packet(params);
        }
    };

    class SetContinuous : public Instruction
    {
      public:
        SetContinuous(byte_t id) : Instruction(id, action::write_data)
        {
          std::vector<byte_t> params;
          params.push_back(ax12::ctrl::cw_angle_limit_lo);
          for (size_t i = 0; i < 4; ++i)
            params.push_back(0);
          rebuild_packet(params);
        }
    };

    class UnsetContinuous : public Instruction
    {
      public:
        UnsetContinuous(byte_t id) : Instruction(id, action::write_data)
        {
          std::vector<byte_t> params;
          params.push_back(ax12::ctrl::cw_angle_limit_lo);
          for (size_t i = 0; i < 2; ++i)
            params.push_back(0);
          params.push_back(255);
          params.push_back(3);
          rebuild_packet(params);
        }
    };

    struct TorqueEnable : public Instruction
    {
      TorqueEnable(byte_t id, bool b) :
        Instruction(id, action::write_data,
                    ctrl::torque_enable, (b ? 1 : 0))
      {
      }
    };

    /*
    Dynamixel can protect itself by detecting errors occur during the operation.
    The errors can be set are as the table below.
    Bit 7 - 0
    Bit 6 - Instruction Error - When undefined Instruction is transmitted or the Action command is delivered without the reg_write command
    Bit 5 - Overload Error - When the current load cannot be controlled with the set maximum torque
    Bit 4 - CheckSum Error - When the Checksum of the transmitted Instruction Packet is invalid
    Bit 3 - Range Error - When the command is given beyond the range of usage
    Bit 2 - OverHeating Error - When the internal temperature is out of the range of operating temperature set in the Control Table
    Bit 1 - Angle Limit Error - When Goal Position is written with the value that is not between CW Angle Limit and CCW Angle Limit
    Bit 0 - Input Voltage Error - When the applied voltage is out of the range of operating voltage set in the Control Table

    If errors occur, in case of Alarm LED (reg 0x11), the LED blinks; in case of Alarm Shutdown (reg 0x12), the motor output becomes 0 % by making the value of Torque Limit(Address 34, 35) as 0.
    */
    struct SetAlarmLed : public Instruction
    {
      SetAlarmLed(byte_t value = 0b01111010) :
        Instruction(broadcast, action::write_data,
                    ctrl::alarm_led, value)
      {
      }
    };
    struct SetAlarmShutdown : public Instruction
    {
      SetAlarmShutdown(byte_t value = 0b01111010) :
        Instruction(broadcast, action::write_data,
                    ctrl::alarm_shutdown, value)
      {
      }
    };

    struct GetPosition : public ReadData
    {
      GetPosition(byte_t id) : ReadData(id, ctrl::present_pos_lo, 2)
      {
      }
    };

    struct GetLoad : public ReadData
    {
        GetLoad(byte_t id) : ReadData(id, ctrl::present_load_lo, 2)
        {
        }
    };

    struct GetVoltage : public ReadData
    {
        GetVoltage(byte_t id) : ReadData(id, ctrl::present_voltage, 1)
        {
        }
    };

//  this is a BROADCAST instruction
    struct ChangeBaudRate : public Instruction
    {
      ChangeBaudRate(byte_t baudrate) :
        Instruction(broadcast, action::write_data,
                    ctrl::baudrate, baudrate)
      {
      }
    };
    struct ChangeId : public Instruction
    {
      ChangeId(byte_t old_id, byte_t new_id) :
        Instruction(old_id, action::write_data,
                    ctrl::id, new_id)
      {
      }
    };

    class SetMaxTorque : public Instruction
    {
      public:
        SetMaxTorque(const byte_t& id, const int torque) : Instruction(id,action::sync_write)
        {
          std::vector<byte_t> params;
          params.push_back(ax12::ctrl::torque_limit_lo);
          params.push_back(2);
          params.push_back(id);
          params.push_back(torque & 0x00FF);
          params.push_back((torque & 0xFF00) >> 8);
          rebuild_packet(params);
        }
    };
  }
}

#endif
