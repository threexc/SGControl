import subprocess
import time
import random as rnd

def control_sg(setup_file, on_file, off_file, duty_cycle=0.05, executable="sg_sequence"):

    #args = ("bin/bar", "-c", "somefile.xml", "-d", "text.txt", "-r", "aString", "-f", "anotherString")
    rnd.seed()
    sg_ip = "134.117.62.53"
    setup_command = ":RAD:AWGN:ARB:STAT 1"
    on_command = "OUTP:STAT 1"
    off_command = "OUTP:STAT 0"
    #on_time = duty_cycle

    #setup_args = ("../{0} {1} {2}".format(executable, sg_ip, setup_file)).split()
    setup_args = ("../{0} {1} {2}".format(executable, sg_ip, setup_command)).split()
    popen = subprocess.Popen(setup_args, stdout=subprocess.PIPE)
    #print("Will now wait for setup_args sequence\n")
    popen.wait()

    while(True):
        #print("inside while loop\n")
        #on_time = rnd.uniform(0.02, 5)
        on_time = rnd.expovariate(0.5)
        off_time = 1 - duty_cycle
        #on_args = ("../{0} {1} {2}".format(executable, sg_ip, on_file))
        on_args = ("../{0} {1} {2}".format(executable, sg_ip, on_command))
        on_args_split = on_args.split()
        #off_args = ("../{0} {1} {2}".format(executable, sg_ip, off_file))
        off_args = ("../{0} {1} {2}".format(executable, sg_ip, off_command))
        off_args_split = off_args.split()
        popen = subprocess.Popen(on_args_split, stdout=subprocess.PIPE)
        popen.wait()
        print(on_args)

        time.sleep(on_time)
        popen = subprocess.Popen(off_args_split, stdout=subprocess.PIPE)
        popen.wait()
        #print(popen.stdout.read())
        print(off_args)
        time.sleep(off_time)


    #output = popen.stdout.read()
    #print(output)

if __name__ == '__main__':
    control_sg("test/control_sequences/awgn_setup.txt", "test/control_sequences/awgn_on.txt", "test/control_sequences/awgn_off.txt", executable="ks_lanio")
