# VolumePopup
This is an application written in c++ and qt 5.5 which provides a volume popup which could remind to os x for windows. 

See the release for a prebuild version. 

Any feedback to this would be apreciated. ( Yeah i know the code is by far not perfect and has still some way to go to count as good but what the hack it works )


This branch is using an event based aproach trying to solve some issues you are running in with the polling approach like show the popup if user tries to change the volume but is already at max / min. Sadly  this runs into the exact same issues because the events are not fired if the value actualy has not changed. besides that is this code prettty ugly and slower ... yet.
