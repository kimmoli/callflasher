Call Flasher
=================

Blinks camera flash during incoming call

Can be toggled on/off via dbus

```dbus-send --session --type=method_call --dest=com.kimmoli.callflasher / com.kimmoli.callflasher.toggle```

A notification is shown if it is enabled or disabled.

Can be forced enabled with

```dbus-send --session --type=method_call --dest=com.kimmoli.callflasher / com.kimmoli.callflasher.enable```

Can be shutdown with 

```dbus-send --session --type=method_call --dest=com.kimmoli.callflasher / com.kimmoli.callflasher.quit```
