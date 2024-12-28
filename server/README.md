https://stackoverflow.com/questions/8169999/how-can-i-create-a-self-signed-cert-for-localhost

export MSYS_NO_PATHCONV=1

openssl req -x509 -sha256 -nodes -newkey rsa:2048 -days 365 -keyout localhost.key -out localhost.crt -subj "/CN=localhost"