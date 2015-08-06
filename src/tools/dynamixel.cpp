#ifdef USE_BOOST

#include <iostream>
#include <algorithm>
#include <dynamixel/dynamixel.hpp>
#include <cmath>
#include <boost/program_options.hpp>

#define READ_DURATION 0.02f
using namespace dynamixel;

std::vector<byte_t> scan(Usb2Dynamixel& controller)
{
    std::cout << "scanning..." << std::endl;
    controller.scan_ax12s();
    const std::vector<byte_t>& ax12_ids = controller.ax12_ids();
    std::cout << "[dynamixel] " << ax12_ids.size()
    << " dynamixel are connected" << std::endl;
    for (size_t i = 0; i < ax12_ids.size(); ++i)
        std::cout << "id:" << (int)ax12_ids[i] << std::endl;
    return ax12_ids;
}

namespace baudrates
{
    static const unsigned b1000000 = 1;
    static const unsigned b115200 = 16;
    static const unsigned b57600 = 34;
}

int find_baudrate(unsigned s)
{
    switch (s)
    {
    case baudrates::b1000000:
        return B1000000;
    case baudrates::b115200:
        return B115200;
    case baudrates::b57600:
        return B57600;
    default:
        std::cerr << "WARNING: uknown baudrate number " << s << std::endl;
        return B115200;
    }
    return B115200;
}

void change_baud(Usb2Dynamixel& controller, unsigned baudrate)
{
    std::cout << "changing baudrate to " << baudrate << std::endl;
    controller.send(ax12::ChangeBaudRate(baudrate));
    std::cout << "done" << std::endl;
}

void change_id(Usb2Dynamixel& controller, unsigned id)
{
    controller.scan_ax12s();
    const std::vector<byte_t>& ax12_ids = controller.ax12_ids();
    std::cout << "[ax12] " << ax12_ids.size()
    << " ax12 are connected" << std::endl;
    if (ax12_ids.size() != 1)
    {
        std::cerr << "you need to connect only ONE dynamixel" << std::endl;
        return;
    }
    std::cout << "changing id of " << ax12_ids[0] << " to " << id << std::endl;
    controller.send(ax12::ChangeId(ax12_ids[0], (byte_t)id));
    dynamixel::Status status;
    controller.recv(READ_DURATION, status);
    usleep(2.5 * 1e5);
    std::cout << "new scan:" << std::endl;
    scan(controller);
}

void zero(Usb2Dynamixel& controller)
{
    std::cout << "setting all dynamixel to zero" << std::endl;
    std::vector<byte_t> ax12_ids = scan(controller);
    dynamixel::Status status;

    for (size_t i = 0; i < ax12_ids.size(); ++i)
    {
        controller.send(dynamixel::ax12::TorqueEnable(ax12_ids[i], true));
        controller.recv(READ_DURATION, status);
    }
    usleep(1e5);
    std::vector<int> pos(ax12_ids.size());

    for (size_t i = 0; i < ax12_ids.size(); ++i)
        if (ax12_ids[i] >= 30) // mx28
            pos[i] = 2048;
        else
            pos[i] = 512;
    controller.send(ax12::SetPositions(ax12_ids, pos));
    std::cout << "done" << std::endl;
}

void init(Usb2Dynamixel& controller)
{
    std::cout << "setting all dynamixel to zero" << std::endl;
    std::vector<byte_t> ax12_ids = scan(controller);
    dynamixel::Status status;

    for (size_t i = 0; i < ax12_ids.size(); ++i)
    {
        controller.send(dynamixel::ax12::TorqueEnable(ax12_ids[i], true));
        controller.recv(READ_DURATION, status);
    }
    usleep(1e5);
    std::vector<int> pos(ax12_ids.size());

    for (size_t i = 0; i < ax12_ids.size(); ++i)
        if (ax12_ids[i] >= 30) // mx28
            pos[i] = 1024;
        else
            pos[i] = 512;
    controller.send(ax12::SetPositions(ax12_ids, pos));


    pos.clear();
    pos.resize(ax12_ids.size());
    sleep(1);
    for (size_t i = 0; i < ax12_ids.size(); ++i)
        switch ((int)ax12_ids[i])
        {
        case 7:
            pos[i] = 700;
            break;
        case 8:
            pos[i] = 700;
            break;
        case 9:
            pos[i] = 1023;
            break;
        case 10:
            pos[i] = 00;
            break;
        case 11:
            pos[i] = 300;
            break;
        case 12:
            pos[i] = 300;
            break;

        default:
            if (ax12_ids[i] >= 30) // mx28
                pos[i] = 1024;
            else
                pos[i] = 512;
            break;
        }


    controller.send(ax12::SetPositions(ax12_ids, pos));

    pos.clear();
    pos.resize(ax12_ids.size());
    sleep(1);
    for (size_t i = 0; i < ax12_ids.size(); ++i)
        switch ((int)ax12_ids[i])
        {
        case 7:
            pos[i] = 700;
            break;
        case 8:
            pos[i] = 700;
            break;
        case 9:
            pos[i] = 1023;
            break;
        case 10:
            pos[i] = 00;
            break;
        case 11:
            pos[i] = 300;
            break;
        case 12:
            pos[i] = 300;
            break;

        default:
            if (ax12_ids[i] >= 30) // mx28
                pos[i] = 2048;
            else
                pos[i] = 512;
            break;
        }


    controller.send(ax12::SetPositions(ax12_ids, pos));



    pos.clear();
    pos.resize(ax12_ids.size());
    sleep(1);
    for (size_t i = 0; i < ax12_ids.size(); ++i)
        if (ax12_ids[i] == 8)
            pos[i] = 350;
        else if (ax12_ids[i] == 11)
            pos[i] = 650;
        else if (ax12_ids[i] >= 30) // mx28
            pos[i] = 2048;
        else
            pos[i] = 512;




    controller.send(ax12::SetPositions(ax12_ids, pos));



    std::cout << "done" << std::endl;
}




void osc(Usb2Dynamixel& controller)
{
    std::cout << "oscillating" << std::endl;
    std::vector<byte_t> ax12_ids = scan(controller);
    dynamixel::Status status;

    for (size_t i = 0; i < ax12_ids.size(); ++i)
    {
        controller.send(dynamixel::ax12::TorqueEnable(ax12_ids[i], true));
        controller.recv(READ_DURATION, status);
    }
    usleep(1e5);
    std::vector<int> pos(ax12_ids.size());
    for (float x = 0; x < 1e10; x += 0.0035)
    {
        for (size_t i = 0; i < ax12_ids.size(); ++i)
            if (ax12_ids[i] >= 30) // mx28
                pos[i] = 2048 + sin(x) * 300;
            else
                pos[i] = 512 + sin(x) * 40;
        controller.send(ax12::SetPositions(ax12_ids, pos));
    }
    std::cout << "done" << std::endl;
}


void positions(Usb2Dynamixel& controller)
{
    std::vector<byte_t> ax12_ids = scan(controller);
    dynamixel::Status status;

    for (size_t i = 0; i < ax12_ids.size(); ++i)
    {
        controller.send(dynamixel::ax12::GetPosition(ax12_ids[i]));
        controller.recv(READ_DURATION, status);
        std::cout << (int)ax12_ids[i] << ":" << status.decode16() << std::endl;
    }

}



void relax(Usb2Dynamixel& controller)
{
    std::cout << "relax..." << std::endl;
    dynamixel::Status status;
    std::vector<byte_t> ax12_ids = scan(controller);
    for (size_t i = 0; i < ax12_ids.size(); ++i)
    {
        controller.send(dynamixel::ax12::TorqueEnable(ax12_ids[i], false));
        controller.recv(READ_DURATION, status);
    }
    std::cout << "done" << std::endl;
}

void resetOverload(Usb2Dynamixel& controller)
{
    std::cout << "reset overload error..." << std::endl;
    dynamixel::Status status;
    std::vector<byte_t> ax12_ids = scan(controller);
    int maxTorque=0;
    for (size_t i = 0; i < ax12_ids.size(); ++i)
    {
        /*controller.send(dynamixel::ax12::ReadData(i,dynamixel::ax12::ctrl::max_torque_lo,2));
        controller.recv(READ_DURATION, status);
        //std::cout << (int)ax12_ids[i] << ":" << status.decode16() << std::endl;
        maxTorque=status.decode16();*/

        //reading present position
        controller.send(dynamixel::ax12::ReadData(ax12_ids[i],dynamixel::ax12::ctrl::present_pos_lo,2));
        controller.recv(READ_DURATION, status);
        byte_t pos_lo=status.get_params()[0];
        byte_t pos_hi=status.get_params()[1];


        //setting goal pos to present pos
        controller.send(dynamixel::ax12::WriteData(ax12_ids[i],dynamixel::ax12::ctrl::goal_position_lo,pos_lo,pos_hi));
        controller.recv(READ_DURATION, status);

        //setting torque limit
        controller.send(dynamixel::ax12::WriteData(ax12_ids[i],dynamixel::ax12::ctrl::torque_limit_lo,255,3));
        controller.recv(READ_DURATION, status);
        std::cout << (int)ax12_ids[i] << ":" << status << std::endl;

        usleep(10);
        controller.send(dynamixel::ax12::ReadData(ax12_ids[i],dynamixel::ax12::ctrl::present_load_lo,2));
        controller.recv(READ_DURATION, status);
        std::cout << (int)ax12_ids[i] << ":" << status << std::endl;
        std::cout <<  "reset id:" <<(int)ax12_ids[i] << std::endl;
    }
    std::cout << "done" << std::endl;
}


void getVersion(Usb2Dynamixel& controller)
{
    std::cout << "check FW version..." << std::endl;
    dynamixel::Status status;
    std::vector<byte_t> ax12_ids = scan(controller);

    for (size_t i = 0; i < ax12_ids.size(); ++i)
    {
        controller.send(dynamixel::ax12::ReadData(ax12_ids[i],dynamixel::ax12::ctrl::firmware_version,1));
        controller.recv(READ_DURATION, status);
        std::cout << (int)ax12_ids[i] << " firmeware version :" << (int)status.get_params()[0] << std::endl;


    }
    std::cout << "done" << std::endl;
}

void getTorque(Usb2Dynamixel& controller, unsigned id)
{
    std::cout << "read present torque..."<< std::endl;
    dynamixel::Status status;


    while (1)
    {
        controller.send(dynamixel::ax12::ReadData(id,dynamixel::ax12::ctrl::present_load_lo,2));
        controller.recv(READ_DURATION, status);
        std::cout << id << " present Torque :" << (int)status.decode16() << std::endl;


    }
    std::cout << "done" << std::endl;
}

void testRoues(Usb2Dynamixel& controller,unsigned  arg)
{
    int dir=arg-512;

    std::cout << "testing wheels' direction..."<< std::endl;
    dynamixel::Status status;

    std::vector<byte_t> idsinit(12);
    idsinit[0]=21;
    idsinit[1]=22;
    idsinit[2]=23;
    idsinit[3]=24;
    idsinit[4]=25;
    idsinit[5]=26;

    idsinit[6]=31;
    idsinit[7]=32;
    idsinit[8]=33;
    idsinit[9]=34;
    idsinit[10]=35;
    idsinit[11]=36;

    std::vector<int> posinit(12);
    posinit[0]=512;
    posinit[1]=512;
    posinit[2]=512;
    posinit[3]=512;
    posinit[4]=512;
    posinit[5]=512;

    posinit[6]=2048+50;
    posinit[7]=2048;
    posinit[8]=2048;
    posinit[9]=2048-150;
    posinit[10]=2048-200;
    posinit[11]=2048;

        controller.send(ax12::SetPositions(idsinit, posinit));
        controller.recv(READ_DURATION, status);


    std::vector<byte_t> ids(6);
    ids[0]=13;
    ids[1]=14;
    ids[2]=15;
    ids[3]=16;
    ids[4]=17;
    ids[5]=18;


    std::vector<bool> direction(6);
    if(dir<256 && dir>-255)
    {
    direction[0]=true;
    direction[1]=true;
    direction[2]=true;
    direction[3]=true;
    direction[4]=true;
    direction[5]=true;
    }
    else{
        direction[0]=false;
    direction[1]=false;
    direction[2]=false;
    direction[3]=false;
    direction[4]=false;
    direction[5]=false;
    }

    std::vector<int> pos(6);

    pos[0]=200;
    pos[1]=200;
    pos[2]=200;
    pos[3]=200;
    pos[4]=200;
    pos[5]=200;

    controller.send(ax12::SetSpeeds(ids, direction,pos));
    controller.recv(READ_DURATION, status);

    ids[0]=7;
    ids[1]=8;
    ids[2]=9;
    ids[3]=10;
    ids[4]=11;
    ids[5]=12;

    if(dir>256)
    dir=dir-512;
    else if(dir<-256)
    dir=dir+512;



    while (1)
    {
        controller.send(dynamixel::ax12::ReadData(1,dynamixel::ax12::ctrl::present_pos_lo,2));
        controller.recv(READ_DURATION, status);
        std::cout <<"1 :  present Position :" << (int)status.decode16() << std::endl;
        pos[0]=512+(int)status.decode16()+dir;

        controller.send(dynamixel::ax12::ReadData(2,dynamixel::ax12::ctrl::present_pos_lo,2));
        controller.recv(READ_DURATION, status);
        std::cout <<"2 :  present Position :" << (int)status.decode16() << std::endl;
        pos[1]=512+(int)status.decode16()-150+dir;

        controller.send(dynamixel::ax12::ReadData(3,dynamixel::ax12::ctrl::present_pos_lo,2));
        controller.recv(READ_DURATION, status);
        std::cout <<"3 :  present Position :" << (int)status.decode16() << std::endl;
        pos[2]=512+(int)status.decode16()+dir;

        controller.send(dynamixel::ax12::ReadData(4,dynamixel::ax12::ctrl::present_pos_lo,2));
        controller.recv(READ_DURATION, status);
        std::cout <<"4 :  present Position :" << (int)status.decode16() << std::endl;
        pos[3]=(int)status.decode16()+dir;

        controller.send(dynamixel::ax12::ReadData(5,dynamixel::ax12::ctrl::present_pos_lo,2));
        controller.recv(READ_DURATION, status);
        std::cout <<"5 :  present Position :" << (int)status.decode16() << std::endl;
        pos[4]=(int)status.decode16()+150+dir;

        controller.send(dynamixel::ax12::ReadData(6,dynamixel::ax12::ctrl::present_pos_lo,2));
        controller.recv(READ_DURATION, status);
        std::cout <<"6 :  present Position :" << (int)status.decode16() << std::endl;
        pos[5]=(int)status.decode16()+dir;

        controller.send(ax12::SetPositions(ids, pos));
        controller.recv(READ_DURATION, status);
    }
    std::cout << "done" << std::endl;

}


void getContacts(Usb2Dynamixel& controller)
{
    std::cout << "getting contacts..."<< std::endl;
    dynamixel::Status status;


    while (1)
    {
        std::cout<<std::endl<<std::endl;

        controller.send(dynamixel::ax12::ReadData(31,dynamixel::ax12::ctrl::present_load_lo,2));
        controller.recv(READ_DURATION, status);
        if ((int)status.decode16()>1024)
            std::cout << " leg 0  :" << "contact" << std::endl;
        else
            std::cout << " leg 0  :" << "no contact\t" <<(int)status.decode16() << std::endl;
        controller.send(dynamixel::ax12::ReadData(32,dynamixel::ax12::ctrl::present_load_lo,2));
        controller.recv(READ_DURATION, status);
        if ((int)status.decode16()>1024)
            std::cout << " leg 1  :" << "contact" << std::endl;
        else
            std::cout << " leg 1  :" << "no contact\t" <<(int)status.decode16() << std::endl;

        controller.send(dynamixel::ax12::ReadData(33,dynamixel::ax12::ctrl::present_load_lo,2));
        controller.recv(READ_DURATION, status);
        if ((int)status.decode16()>1024)
            std::cout << " leg 2  :" << "contact" << std::endl;
        else
            std::cout << " leg 2  :" << "no contact\t" <<(int)status.decode16()<< std::endl;

        controller.send(dynamixel::ax12::ReadData(34,dynamixel::ax12::ctrl::present_load_lo,2));
        controller.recv(READ_DURATION, status);
        if ((int)status.decode16()>1024)
            std::cout << " leg 3  :" << "contact" << std::endl;
        else
            std::cout << " leg 3  :" << "no contact\t" <<(int)status.decode16() << std::endl;

        controller.send(dynamixel::ax12::ReadData(35,dynamixel::ax12::ctrl::present_load_lo,2));
        controller.recv(READ_DURATION, status);
        if ((int)status.decode16()>1024)
            std::cout << " leg 4  :" << "contact" << std::endl;
        else
            std::cout << " leg 4  :" << "no contact\t" <<(int)status.decode16()<< std::endl;

        controller.send(dynamixel::ax12::ReadData(36,dynamixel::ax12::ctrl::present_load_lo,2));
        controller.recv(READ_DURATION, status);
        if ((int)status.decode16()>1024)
            std::cout << " leg 5  :" << "contact" << std::endl;
        else
            std::cout << " leg 5  :" << "no contact\t" <<(int)status.decode16() << std::endl;

        usleep(1e5);
    }
    std::cout << "done" << std::endl;
}

/** Set the control mode of an actuator to continuous.
  *
  * It allows the servo-motor for continuous rotation and disables position
  * commands.
  * @param controller reference to the controller object
  * @param id id of the actuator affected
  */
void continuous_mode(Usb2Dynamixel& controller, int id)
{
  std::vector<byte_t> servos = scan(controller);
  if (std::find(servos.begin(), servos.end(), (byte_t)id) == servos.end()) {
    std::cout << "The requested ID corresponds to none of the connected "
      << "servo-motors." << std::endl;
      return;
  }

  dynamixel::Status status;

  // Set the control mode to continuous
  std::cout << "Set control mode to continuous" << std::endl;
  controller.send(dynamixel::ax12::UnsetContinuous((byte_t)id));
  controller.recv(READ_DURATION, status);
  std::cout << "done" << std::endl;
}


/** Set the control mode of an actuator to position.
  *
  * It makes the servo-motor move to given positions. Reverts the effect of
  * `continuous_mode`. In this mode, defining the moving speed affect the speed
  * at which the servo-motor moves to its target.
  * @param controller reference to the controller object
  * @param id id of the actuator affected
  */
void position_mode(Usb2Dynamixel& controller, int id)
{
  std::vector<byte_t> servos = scan(controller);
  if (std::find(servos.begin(), servos.end(), (byte_t)id) == servos.end()) {
    std::cout << "The requested ID corresponds to none of the connected "
      << "servo-motors." << std::endl;
      return;
  }

  dynamixel::Status status;

  // Set the control mode to continuous
  std::cout << "Set control mode to continuous" << std::endl;
  controller.send(dynamixel::ax12::SetContinuous((byte_t)id));
  controller.recv(READ_DURATION, status);
  std::cout << "done" << std::endl;
}


void select_command(const std::string& command, unsigned arg,
                    Usb2Dynamixel& controller)
{
  if (command == "scan")
    scan(controller);
  else
  if (command == "change_baud")
    change_baud(controller, arg);
  else
  if (command == "change_id")
    change_id(controller, arg);
  else
  if (command == "zero")
    zero(controller);
  else
  if (command == "relax")
    relax(controller);
  else
  if (command == "reset_overload")
    resetOverload(controller);
  else
  if (command == "get_torque")
    getTorque(controller,arg);
  else
  if (command == "get_version")
    getVersion(controller);
  else
  if (command == "get_contacts")
    getContacts(controller);
  else
  if (command == "init")
    init(controller);
  else
  if (command == "test_roues")
    testRoues(controller,arg);
  else
  if (command == "positions")
    positions(controller);
  else
  if (command == "osc")
    osc(controller);
  else
  if (command == "continuous_mode")
    continuous_mode(controller, arg);
  else
  if (command == "position_mode")
    position_mode(controller, arg);
  else
    std::cerr << "unknown command:" << command
    << " (arg=" << arg << ")" << std::endl;
}

int main(int argc, char **argv)
{
    std::string port = "/dev/ttyUSB0";
    int baudrate = B1000000;
    std::string command = "scan";
    unsigned arg = 0;
    namespace po = boost::program_options;
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help,h", "produce help message")
    ("port,p", po::value<std::string>(), "port")
    ("baudrate,b", po::value<unsigned>(),
     "baud rate for the communication (1=1Mb, 2=115200, 34=57600)")
    ("command,c", po::value<std::string>(),
     "command [scan, zero, init , change_baud, change_id, relax, get_contacts, test_roues, positions, reset_overload, get_version, get_torque, osc, continuous_mode, position_mode]")
    ("arg,a", po::value<unsigned>(), "argument of the command")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }
    if (vm.count("port"))
        port = vm["port"].as<std::string>();
    if (vm.count("baudrate"))
        baudrate = find_baudrate(vm["baudrate"].as<unsigned>());
    if (vm.count("command"))
        command = vm["command"].as<std::string>();
    if (vm.count("arg"))
        arg = vm["arg"].as<unsigned>();

    std::cout << "port: :" << port << std::endl;
    std::cout << "baudrate (code): " << baudrate << std::endl;
    std::cout << "command:[" << command << "]" << std::endl;
    std::cout << "arg:[" << arg << "]" << std::endl;

    try
    {
        std::cout << "opening serial..." << std::endl;
        Usb2Dynamixel controller;
        controller.open_serial(port, baudrate);
        std::cout << "serial port open" << std::endl;
        select_command(command, arg, controller);
    }
    catch (Error e)
    {
        std::cerr << "error (dynamixel): " << e.msg() << std::endl;
    }
    return 0;
}
#else
#include <iostream>
int main()
{
    std::cerr << "boost is required to compile this tool" << std::endl;
}
#endif
