sudo mosquitto_pub -h 127.0.0.1 -t my/awesome/topic -u kokos -P testpswd -m "{\"cmd\": \"forward\", \"val\": \"5\"}"
