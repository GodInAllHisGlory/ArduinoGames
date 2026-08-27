const char indexHtml[] = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    .matrix{
      padding: 8px;
    }
    .row{
      display: flex;
    }

    .led {
      border: none;
      width: 40px;
      height: 40px;
      margin: 5px;
      background: black;
    }
  </style>
  <title>Arduino Games</title>
</head>
  <body>
    <h3>Click and drag on the grid and see it appear on the Arduino</h3>
  <script>
    const HEIGHT = 8;
    const WIDTH = 12;
    const origin = window.location.origin;
    const matrixArray = new Array(WIDTH * HEIGHT).fill(0);
    const body = document.body;
    const matrix = document.createElement('div');
    matrix.className = 'matrix';
    let isDown = false;
    body.addEventListener('mousedown', () => isDown = true);
    body.addEventListener('mouseup', () => {isDown = false; sendMatrix();});

    function sendMatrix(){
        const uintArr = new Array(3);
        let pointer = 0;

        for(let i = 0; i < 3; i++){
            const int32 = parseInt(matrixArray.slice(pointer, pointer+=32).join(''), 2) >>> 0; //converts the binary to a 32 bit unsigend integer
            uintArr[i] = `0x${int32.toString(16)}`; //Converts numbers to hex
        }
        fetch(`${origin}/${uintArr.join('-')}`, {method:'POST'})
    }

    let matrixIndex = 0;
    for(let i = 0; i < HEIGHT; i++){
        const row = document.createElement('div');
        row.className = "row";
        for(let j = 0; j < WIDTH; j++){
            const led = document.createElement('div');
            led.className = "led";
            const ledIndex = matrixIndex;
            led.addEventListener('mouseenter', () => {if(isDown){
                led.style.backgroundColor = led.style.backgroundColor === 'red'? 'black' : 'red'}
                matrixArray[ledIndex] = led.style.backgroundColor === 'red'? 1 : 0;
            });
            matrixIndex++;
            row.append(led);
        }
    matrix.append(row);
    }
    document.body.append(matrix);
  </script>
  </body>
</html>
)rawliteral";