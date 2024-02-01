import './App.scss';
import { Car } from '../_helpers/structs';

function App() {
    var car: Car = {
        Id: 'null',
        sensors: {
            rotation: {
                x: 0,
                y: 0,
                z: 0,
            },
            distance: {
                left: 0,
                center: 0,
                right: 0,
            },
        },
        position: {
            x: 0,
            y: 0,
            z: 0,
        },
    };

    var ws: WebSocket;

    ws = new WebSocket('ws://ws.auto.sillyfrog.nl:8100');

    ws.addEventListener('message', async (e) => {
        var data: string = await e.data.text();

        var dataArray = data.split(',');

        car.Id = dataArray[0];
        car.sensors.distance.left = Number(dataArray[1]);
        car.sensors.distance.center = Number(dataArray[2]);
        car.sensors.distance.right = Number(dataArray[3]);

        console.log(dataArray);

        update();
    });

    function update() {
        document.title = `Auto: ${car.Id}`;

        var arr = ['left', 'center', 'right'];

        // @ts-ignore
        document.getElementById('carId').innerHTML = car.Id;

        arr.forEach((dist) => {
            // @ts-ignore
            document.getElementById(dist).innerHTML =
                // @ts-ignore
                car.sensors.distance[dist].toString();

            // @ts-ignore
            if (car.sensors.distance[dist] > 40.0) {
                // @ts-ignore
                document.getElementById(
                    dist,
                ).parentElement.style.backgroundColor = '#00a551';
                // @ts-ignore
            } else if (car.sensors.distance[dist] > 20.0) {
                // @ts-ignore
                document.getElementById(
                    dist,
                ).parentElement.style.backgroundColor = '#FF9966';
            } else {
                // @ts-ignore
                document.getElementById(
                    dist,
                ).parentElement.style.backgroundColor = '#ff4c4c';
            }
        });
    }

    return (
        <div className="page">
            <header className="header">
                <h1 id="carId">Auto</h1>
            </header>
            <main className="main">
                {/* <div>
                    <p>{car.Id}</p>
                </div>
                <div>
                    
                </div>
                <div></div>
                <button onClick={clickHandler()}>test</button> */}
                <div>
                    <h3>Links</h3>
                    <p id="left">{car.sensors.distance.left}</p>
                </div>
                <div>
                    <h3>Midden</h3>
                    <p id="center">{car.sensors.distance.center}</p>
                </div>
                <div>
                    <h3>Rechts</h3>
                    <p id="right">{car.sensors.distance.right}</p>
                </div>
            </main>
            <footer className="footer">&copy; 2024 Vincent Brokx</footer>
        </div>
    );
}

export default App;
