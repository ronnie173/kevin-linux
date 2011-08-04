# set ip_forward
echo "before set ip_forward"
cat /proc/sys/net/ipv4/ip_forward

echo "1" > /proc/sys/net/ipv4/ip_forward
echo "after set ip_forward"
cat /proc/sys/net/ipv4/ip_forward

