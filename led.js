'use strict';

let ledWrite = null;

// Bind function to text box so that it runs when enter key is pressed
$(document).ready(function(){
    $('#data_field').keypress(function(e){
        if(e.keyCode == 13 || e.which == 13) {
            sendData(-1);
        }
    });
});

// Hide connect button and show text box once connected
function onConnected() {
    document.querySelector('.connect-button').classList.add('hidden');
    document.querySelector('.user_data').classList.remove('hidden');
}

// Connect to LED
function connect() {
    console.log('Requesting Bluetooth Device...');
    navigator.bluetooth.requestDevice({
            filters: [{ name: 'MLT-BT05'}],
            optionalServices: ['0000ffe0-0000-1000-8000-00805f9b34fb']
        })
        .then(device => {
            console.log('> Found ' + device.name);
            console.log('Connecting to GATT Server...');
            return device.gatt.connect();
        })
        .then(server => {
            console.log('Getting Service 0000ffe0-0000-1000-8000-00805f9b34fb...');
            return server.getPrimaryService('0000ffe0-0000-1000-8000-00805f9b34fb');
        })
        .then(service => {
            console.log('Getting Characteristic...');
            return Promise.all([
                service.getCharacteristic('0000ffe1-0000-1000-8000-00805f9b34fb')
            ]);
        })
        .then(characteristic => {
            [ledWrite] = characteristic;
            return ledWrite.startNotifications().then(_ => {
                console.log('> Notifications started');
                ledWrite.addEventListener('characteristicvaluechanged',
                                                handleNotifications);
            });
        })
        .then(() => {
            console.log('All ready!');
            onConnected();
        })
        .catch(error => {
            console.log('Argh! ' + error);
        });
}

// Sends data to device
function sendData(val) {
    if (val < 0) {
        val = $('#data_field').val();
    }

    console.log('Sending status packet (' + val + ')...');
    let data = new Uint8Array([val]);
    return ledWrite.writeValue(data)
        .catch(err => console.log('Error when sending status packet! ', err))
}

// Print notification to console if received
function handleNotifications(event) {
    let value = event.target.value;
    console.log('> ' + value);
}
