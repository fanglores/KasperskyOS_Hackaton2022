printf "\nSet up the network interface\n"

ip tuntap add tap0 mode tap
ip addres add 10.0.2.2/22 dev tap0
ip link set tap0 up

printf "\nSet up mosquitto broker\n" 

sudo cp ${PWD}/../mosquitto.conf /etc/mosquitto/mosquitto.conf
sudo mosquitto -c /etc/mosquitto/mosquitto.conf -v

