printf "\nPublish a message\n"

sudo mosquitto_pub -h 10.0.2.2 -t my/awesome/topic -m "hello"

printf "\nStart subscriber application:\n"

/opt/KasperskyOS-Community-Edition-1.1.0.356/toolchain/bin/qemu-system-aarch64 -m 2048 -machine vexpress-a15 -nographic -monitor none -serial stdio -netdev tap,id=net0,ifname=tap0,script=no,downscript=no -net nic,model=lan9118,netdev=net0 -kernel ${PWD}/build/einit/kos-qemu-image
