#!/usr/bin/env python3
import rospy
from floor_msg.msg  import Floor  
import sys, select, termios, tty
import time

msg = """
Control Your CAR!
Reading from the keyboard  and Publishing to Floor!
---------------------------
Moving around:
   u    i    o
   j    k    l
   m    ,    .

space key, x : force stop
c:  open horn
v:  open headlight

w/s: shift the middle pos of throttle by +/- 5 pwm
a/d: shift the middle pos of steering by +/- 2 pwm

CTRL-C to quit
"""
moveBindings = {
        'i':(1,0),
        'o':(1,-1),
        'j':(0,1),
        'l':(0,-1),
        'u':(1,1),
        ',':(-1,0),
        '.':(-1,-1),
        'm':(-1,1),
        'I':(1,0),
        'O':(1,-1),
        'J':(0,1),
        'L':(0,-1),
        'U':(1,1),
        '<':(-1,0),
        '>':(-1,-1),
        'M':(-1,1),
           }
def getKey():
    tty.setraw(sys.stdin.fileno())
    rlist, _, _ = select.select([sys.stdin], [], [], 0.1)
    if rlist:
        key = sys.stdin.read(1)
    else:
        key = ''
    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
    return key

def vels(speed,turn,headlight,horn,stop):
    print("currently:\tspeed %s\tturn %s\tstop %s\thorn %s\theadlight %s" % (speed,turn,stop,horn,headlight) ) 

def cmd_callback(data):
    global speed_bool
    global speed_output
    global turn_output
    global turn_dir
    global headlightttt
    global laba
    global stopppp
    stopppp=data.stop
    laba=data.horn
    headlightttt=data.headlight
    if data.speed>=0:
        speed_bool=1
        speed_output=data.speed
    else:
        speed_bool=-1
        speed_output=-0
    if data.angle>=0:
        turn_dir=1
        turn_output=data.angle
    else:
        turn_dir=-1
        turn_output=-data.angle
    
if __name__=="__main__":
    settings = termios.tcgetattr(sys.stdin)
    speed_start_value = 510;speed_mid = 1500;speed_bias = 0;speed_add_once = 20 # 起始速度\速度中值\速度偏移量\速度调整幅值    
    control_speed = speed_mid   #实际运行速度值，初值为中值
    turn_start_value = 60;turn_mid = 90;turn_bias = 0;turn_add_once = 2; # 起始角度\角度中值\角度偏移量\角度调整幅值
    control_turn = turn_mid   #实际运行角度值，初值为中值  
    #################################定义话题
    rospy.init_node('car_teleop')
    sub=rospy.Subscriber('/cmd',Floor,cmd_callback)
    pub = rospy.Publisher('~/car_vel', Floor, queue_size=5)
    twist = Floor()    
    #################################    
    speed_dir = 0    #本次速度值方向
    last_speed_dir = 0    #上一次速度值方向,为的是反转做个缓冲
    run = 0       
    
    speed = speed_mid;angle = turn_mid
    headlight = headlightttt;horn = laba;stop = stopppp
    try:
        while(1):
            key = getKey()
            speed_dir=speed_bool
            speed_bias=speed_output #最大值1500
            turn_bias=turn_output  #最大值80
            #左转为正
            #speed_dir为正还是反（1，0，-1），speed_bias为控制输出
            if(speed_dir!=0 and speed_dir + last_speed_dir == 0):#防止突然电机速度反转
                    control_speed = speed_mid                 
                    last_speed_dir = speed_dir
                    control_turn = turn_mid    
            else:             
                    control_speed = speed_dir * (speed_start_value + speed_bias) + speed_mid    
                    control_turn = turn_dir * (turn_start_value + turn_bias)+ turn_mid

            # ##############################################发布话题
            # if(run == 1):                      
            twist.speed = control_speed; twist.angle = control_turn 
            twist.headlight = headlight;twist.horn = horn; twist.stop = stop
            pub.publish(twist)  
            vels(control_speed,control_turn,headlight,horn,stop)                 
            horn = 0;stop = 0                
            # else:
            #     twist.speed = speed_mid; twist.angle = turn_mid 
            #     twist.headlight = headlight;twist.horn = horn; twist.stop = stop
            #     pub.publish(twist)                           
            #     vels(speed_mid,turn_mid,headlight,horn,stop)
            #     horn = 0;stop = 0    
                
            if (key == '\x03'):   #for ctrl + c exit
                break            
    except:
        print ("error")

    finally:
        twist.speed = speed_mid; twist.angle = turn_mid 
        twist.headlight = 0;twist.horn = 0; twist.stop = 0
        pub.publish(twist) 
        vels(speed_mid,turn_mid,0,0,0)

    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
