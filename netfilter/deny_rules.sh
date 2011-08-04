# Parameter settings
  EXTIF="eth1"           # To public interface
  INIF="eth2"            # To internal interface
  INNET="10.11.0.0/16" # Internal subnet
  export EXTIF INIF INNET


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

# 6. save the config
  iptables-save
