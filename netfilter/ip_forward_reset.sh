# unset ip_forward
echo "0" > /proc/sys/net/ipv4/ip_forward
echo "ip_forward"
cat /proc/sys/net/ipv4/ip_forward

