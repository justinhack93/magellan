#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <termios.h>

int getch()
{
  static struct termios oldt, newt;
  tcgetattr( STDIN_FILENO, &oldt);           // save old settings
  newt = oldt;
  newt.c_lflag &= ~(ICANON);                 // disable buffering      
  tcsetattr( STDIN_FILENO, TCSANOW, &newt);  // apply new settings

  int c = getchar();  // read character (non-blocking)

  tcsetattr( STDIN_FILENO, TCSANOW, &oldt);  // restore old settings
  return c;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "teleop_key");
  ros::NodeHandle n;
  ros::Publisher vel_pub_ = n.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 10);

  while (ros::ok())
  {
    geometry_msgs::Twist twist;
    int c = getch();   // call your non-blocking input function
    switch (c) {
    case 'w':
      twist.linear.x = 1;
      break;
    case 'a':
      twist.angular.z = 1;
      break;
    case 's':
      twist.linear.x = -1;
      break;
    case 'd':
      twist.angular.z = -1;
      break;
    }

    vel_pub_.publish(twist);
    ros::spinOnce();
  }

  return 0;
}
