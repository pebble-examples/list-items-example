# list-items-example

Simple example app that demonstrates how to efficiently send a list of values at
high speed. When JS is ready, it will send a list of items to the C side. When
the transfer is complete, the C side will send a list back to JS.

This is achieved though intelligent use of the `AppMessage` callbacks on both
sides without using timers, which can prove unreliable. As soon as each side's
success callback is called, the developer is free to send the next message.