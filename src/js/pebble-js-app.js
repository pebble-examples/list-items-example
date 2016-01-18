var dataItems = [ 2, 4, 8, 16, 32, 64 ];
var NUM_ITEMS = dataItems.length;

function sendItem(items, index) {
  // Build message
  var key = 'AppKeyItem' + index;
  var dict = {};
  dict[key] = items[index];

  // Send the message
  Pebble.sendAppMessage(dict, function() {
    // Use success callback to increment index
    index++;

    if(index < NUM_ITEMS) {
      // Send next item
      sendItem(items, index);
    }
  }, function() {
    console.log('Item transmission failed: ' + index);
  });
}

function sendList(items) {
  var index = 0;
  sendItem(items, index);
}

Pebble.addEventListener('ready', function() {
  console.log('PebbleKit JS ready!');

  // Begin list transmission
  sendList(dataItems);
});

Pebble.addEventListener('appmessage', function(e) {
  var dict = e.payload;

  for(var index = 0; index < NUM_ITEMS; index++) {
    var key = 'AppKeyItem' + index;
    if(dict[key] !== undefined) {
      // This key was received
      console.log('C sent item: ' + dict[key]);
    }
  }
});