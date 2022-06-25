#!/bin/sh
set -e
. ./iso.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom myos.iso -gdb tcp::26000 -S -d int,cpu_reset -D qemu2.log -m 2000m
