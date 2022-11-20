sudo mount /dev/mmcblk0p1 ~/mnt/fat32
sudo cp build/einit/kos-image ~/mnt/fat32/kos-image
sync

sudo umount ~/mnt/fat32
