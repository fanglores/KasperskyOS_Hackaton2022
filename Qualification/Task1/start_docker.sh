sudo docker run -it -v ${PWD}/shared:/workspace --device /dev/net/tun:/dev/net/tun --privileged --rm kos bash
