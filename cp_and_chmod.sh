#!/bin/bash

# Copy the read_button executable to the cgicc directory
echo "Copying the application to the cgi-bin directory..."
sudo cp myLed.cgi /usr/lib/cgi-bin/

# Set permissions
echo "Setting permissions..."
sudo chmod +s /usr/lib/cgi-bin/myLed.cgi
echo "success"
