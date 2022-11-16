const haveEvents = 'ongamepadconnected' in window;
const controllers = {};
const joystick = [0,0,0,0];

function connecthandler(e) {
  addgamepad(e.gamepad);
}

function addgamepad(gamepad) {
  controllers[gamepad.index] = gamepad;

  // See https://github.com/luser/gamepadtest/blob/master/index.html
  const start = document.getElementById("start");
  requestAnimationFrame(updateStatus);
}

function disconnecthandler(e) {
  removegamepad(e.gamepad);
}

function removegamepad(gamepad) {
  delete controllers[gamepad.index];
}

function updateStatus() {
  if (!haveEvents) {
    scangamepads();
  }

let i=0;
// console.log(controllers[0]);

if(controllers.length>1){
  for (const controller of controllers){
}
}else{
  controller = controllers[0];

  controller.buttons.forEach((button, i) => {
    if(button.pressed == true){
      console.log(i);
    }});


  let x_move = parseFloat(controller.axes[0]);
  let y_move = parseFloat(controller.axes[1]);
  if(Math.abs(y_move) >0.05 || Math.abs(x_move) >0.05){
    moveJoystick(0, x_move, y_move, false, undefined);
  }

  if(Math.abs(parseFloat(controller.axes[2])) >0.05 || parseFloat(controller.axes[3]) >0.05){
    moveJoystick(1, parseFloat(controller.axes[2]), parseFloat(controller.axes[3]), false, undefined);
  }
    if(Math.abs(joystick[0]-controller.axes[0])>0.08 || Math.abs(joystick[1]-controller.axes[1])>0.08){
      if(Math.abs(controller.axes[0]<0.05) && Math.abs(controller.axes[1]<0.05)){
        console.log('stopping cube 1');
        console.log(Math.abs(joystick[0]));
        console.log(Math.abs(joystick[1]));
        setTimeout(function(){
      stopping(0);

      }, 200);

      }
   }

   if(Math.abs(joystick[2]-controller.axes[2])>0.08 || Math.abs(joystick[3]-controller.axes[3])>0.08){
     if(Math.abs(controller.axes[2]<0.05) && Math.abs(controller.axes[3]<0.05)){
       console.log('stopping cube 2');
       setTimeout(function(){
     stopping(1);

     }, 200);

     }
  }
  controller.axes.forEach((axis, i) => {
       joystick[i] = controller.axes[i]
  });





  requestAnimationFrame(updateStatus);
}
}




function scangamepads() {
  const gamepads = navigator.getGamepads();
  for (const gamepad of gamepads) {
    if (gamepad) { // Can be null if disconnected during the session
      if (gamepad.index in controllers) {
        controllers[gamepad.index] = gamepad;
      } else {
        addgamepad(gamepad);
      }
    }
  }
}

window.addEventListener("gamepadconnected", connecthandler);
window.addEventListener("gamepaddisconnected", disconnecthandler);

if (!haveEvents) {
 setInterval(scangamepads, 1000);
 console.log('scanning gamepad')
}
