#!/bin/bash

# Parameter settings
  EXTIF="eth4"           # To public interface
  INIF="eth5"            # To internal interface
  INNET="10.11.0.0/16" # Internal subnet
  export EXTIF INIF INNET

# Part one, firewall unit settings
# 1. kernel network settings
#  echo "1" > /proc/sys/net/ipv4/tcp_syncookies
#  echo "1" > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts
#  for i in /proc/sys/net/ipv4/conf/*/{rp_filter,log_martians}; do
#        echo "1" > $i
#  done
#  for i in /proc/sys/net/ipv4/conf/*/{,accept_source_route,accept_redirects,\
#send_redirects}; do
#        echo "0" > $i
#  done

# 2. clear rule, predefine policy and lo related settings
  PATH=/sbin:/usr/sbin:/bin:/usr/bin:/usr/local/sbin:/usr/local/bin; export PATH
  iptables -F
  iptables -X
  iptables -Z
  iptables -P INPUT   DROP

iptables -A INPUT -p TCP -i $EXTIF --dport  121 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  221 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  321 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  421 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  521 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  621 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  721 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  821 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  921 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  211 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  221 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  231 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  241 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  251 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  261 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  271 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  281 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  291 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  211 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  212 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  213 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  214 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  215 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  216 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  217 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  218 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  219 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  1121 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  1221 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  1321 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  1421 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  1521 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  1621 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  1721 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  1821 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  1921 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  2121 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  2221 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  2321 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  2421 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  2521 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  2621 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  2721 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  2821 --sport 1024:65534 -j REJECT # FTP
iptables -A INPUT -p TCP -i $EXTIF --dport  2921 --sport 1024:65534 -j REJECT # FTP

  iptables -P OUTPUT  ACCEPT
  iptables -P FORWARD ACCEPT
  iptables -A INPUT -i lo -j ACCEPT
  iptables -A INPUT -m state --state RELATED,ESTABLISHED -j ACCEPT

# 3. apply additional firewall scripts
#  if [ -f /usr/local/virus/iptables/iptables.deny ]; then
#        sh /usr/local/virus/iptables/iptables.deny
#  fi
#  if [ -f /usr/local/virus/iptables/iptables.allow ]; then
#        sh /usr/local/virus/iptables/iptables.allow
#  fi
#  if [ -f /usr/local/virus/httpd-err/iptables.http ]; then
#        sh /usr/local/virus/httpd-err/iptables.http
#  fi

# 4. allow certain ICMP
  AICMP="0 3 3/4 4 11 12 14 16 18"
  for tyicmp in $AICMP
  do
    iptables -A INPUT -i $EXTIF -p icmp --icmp-type $tyicmp -j ACCEPT
  done

# 5. allow certain services
# iptables -A INPUT -p TCP -i $EXTIF --dport  21 --sport 1024:65534 -j ACCEPT # FTP
# iptables -A INPUT -p TCP -i $EXTIF --dport  22 --sport 1024:65534 -j ACCEPT # SSH
# iptables -A INPUT -p TCP -i $EXTIF --dport  25 --sport 1024:65534 -j ACCEPT # SMTP
# iptables -A INPUT -p UDP -i $EXTIF --dport  53 --sport 1024:65534 -j ACCEPT # DNS
# iptables -A INPUT -p TCP -i $EXTIF --dport  53 --sport 1024:65534 -j ACCEPT # DNS
# iptables -A INPUT -p TCP -i $EXTIF --dport  80 --sport 1024:65534 -j ACCEPT # WWW
# iptables -A INPUT -p TCP -i $EXTIF --dport 110 --sport 1024:65534 -j ACCEPT # POP3
# iptables -A INPUT -p TCP -i $EXTIF --dport 443 --sport 1024:65534 -j ACCEPT # HTTPS


# Part two, firewall setting for internal network
# 1. load modules
#  modules="ip_tables iptable_nat ip_nat_ftp ip_nat_irc ip_conntrack 
#ip_conntrack_ftp ip_conntrack_irc"
#  for mod in $modules
#  do
#      testmod=`lsmod | grep "^${mod} " | awk '{print $1}'`
#      if [ "$testmod" == "" ]; then
#            modprobe $mod
#      fi
#  done

# 2. clear NAT table rules
  iptables -F -t nat
  iptables -X -t nat
  iptables -Z -t nat
  iptables -t nat -P PREROUTING  ACCEPT
  iptables -t nat -P POSTROUTING ACCEPT
  iptables -t nat -P OUTPUT      ACCEPT

# 3. turn on routing
  if [ "$INIF" != "" ]; then
    iptables -A INPUT -i $INIF -j ACCEPT
    echo "1" > /proc/sys/net/ipv4/ip_forward
    if [ "$INNET" != "" ]; then
        for innet in $INNET
        do
            #iptables -t nat -A POSTROUTING -s $innet -o $EXTIF -j MASQUERADE
            iptables -t nat -A POSTROUTING -s $innet -o $EXTIF -j SNAT --to 10.21.13.1-10.21.13.200
        done
    fi
  fi
  # If there are connection problems to MSN or certain website£¬
  # that could be a MTU issue, uncomment following line to load MTU limits
  # iptables -A FORWARD -p tcp -m tcp --tcp-flags SYN,RST SYN -m tcpmss \
  #          --mss 1400:1536 -j TCPMSS --clamp-mss-to-pmtu

# 4. Expose server service to outside
# iptables -t nat -A PREROUTING -p tcp -i $EXTIF --dport 80 \
#          -j DNAT --to-destination 192.168.1.210:80 # WWW

# 5. special requirements, such as Windows terminal
# iptables -t nat -A PREROUTING -p tcp -s 1.2.3.4  --dport 6000 \
#          -j DNAT --to-destination 192.168.1.100
# iptables -t nat -A PREROUTING -p tcp -s 1.2.3.4  --sport 3389 \
#          -j DNAT --to-destination 192.168.1.100

# 6. save the config
  iptables-save
