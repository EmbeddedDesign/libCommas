#!/bin/bash

temp_key_file=temp_key_file.h

#Generate CA Keys
sizePub=0
sizePrv=0
while [ "$sizePub" != 128 ] || [ "$sizePrv" != 64 ]; do
openssl ecparam -genkey -name prime256v1 -out CAprivate.pem
openssl ec -in CAprivate.pem -pubout -out CApublic.pem 2>/dev/null
CAPublicKey=$(openssl ec -in CAprivate.pem -noout -text 2> /dev/null | awk '{$1=$1}{ print }' | sed ':a;N;$!ba;s/\n//g' | sed -e 's/^.*04://g;s/ASN.*$//g' | sed -e 's/://g')
CAPrivateKey=$(openssl ec -in CAprivate.pem -noout -text 2> /dev/null | awk '{$1=$1}{ print }' | sed ':a;N;$!ba;s/\n//g' | sed -e 's/^.*priv://g;s/pub.*$//g' | sed -e 's/://g')
sizePub=${#CAPublicKey}
sizePrv=${#CAPrivateKey}
done

#Generate Border Router Keys
sizePub=0
sizePrv=0
while [ "$sizePub" != 128 ] || [ "$sizePrv" != 64 ]; do
openssl ecparam -genkey -name prime256v1 -out BRprivate.pem
openssl ec -in BRprivate.pem -pubout -out BRpublic.pem 2>/dev/null
BRPublicKey=$(openssl ec -in BRprivate.pem -noout -text 2> /dev/null | awk '{$1=$1}{ print }' | sed ':a;N;$!ba;s/\n//g' | sed -e 's/^.*04://g;s/ASN.*$//g' | sed -e 's/://g')
BRPrivateKey=$(openssl ec -in BRprivate.pem -noout -text 2> /dev/null | awk '{$1=$1}{ print }' | sed ':a;N;$!ba;s/\n//g' | sed -e 's/^.*priv://g;s/pub.*$//g' | sed -e 's/://g')
sizePub=${#BRPublicKey}
sizePrv=${#BRPrivateKey}
done

#Generate End Device Keys
sizePub=0
sizePrv=0
while [ "$sizePub" != 128 ] || [ "$sizePrv" != 64 ]; do
openssl ecparam -genkey -name prime256v1 -out EDprivate.pem
openssl ec -in EDprivate.pem -pubout -out EDpublic.pem 2>/dev/null
EDPublicKey=$(openssl ec -in EDprivate.pem -noout -text 2> /dev/null | awk '{$1=$1}{ print }' | sed ':a;N;$!ba;s/\n//g' | sed -e 's/^.*04://g;s/ASN.*$//g' | sed -e 's/://g')
EDPrivateKey=$(openssl ec -in EDprivate.pem -noout -text 2> /dev/null | awk '{$1=$1}{ print }' | sed ':a;N;$!ba;s/\n//g' | sed -e 's/^.*priv://g;s/pub.*$//g' | sed -e 's/://g')
sizePub=${#EDPublicKey}
sizePrv=${#EDPrivateKey}
done

#Generate UUIDs
sizeEDUUID=0
sizeBRUUID=0
while [ "$sizeEDUUID" != 36 ] || [ "$sizeBRUUID" != 36 ]; do
EDUUID=$(uuidgen)
BRUUID=$(uuidgen)
sizeEDUUID=${#EDUUID}
sizeBRUUID=${#BRUUID}
done

echo "// CA keys" > "$temp_key_file"
echo "char CAPublicKey[] = \"$CAPublicKey\";" >> "$temp_key_file"
echo "char CAPrivateKey[] = \"$CAPrivateKey\";" >> "$temp_key_file"

echo "// End Device keys" >> "$temp_key_file"
echo "char EDPublicKey[] = \"$EDPublicKey\";" >> "$temp_key_file"
echo "char EDPrivateKey[] = \"$EDPrivateKey\";" >> "$temp_key_file"
echo "char EDUUID[] = \"$EDUUID\";" >> "$temp_key_file"

echo "// Border Router keys" >> "$temp_key_file"
echo "char BRPublicKey[] = \"$BRPublicKey\";" >> "$temp_key_file"
echo "char BRPrivateKey[] = \"$BRPrivateKey\";" >> "$temp_key_file"
echo "char BRUUID[] = \"$BRUUID\";" >> "$temp_key_file"

rm -f *.pem *.sig *.pub