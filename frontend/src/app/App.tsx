import './App.scss';
import { car, openWebsocket, sendMessage } from '../_helpers/wsConn';

function App() {
    openWebsocket();

    const clickHandler = () => {
        return (event: React.MouseEvent) => {
            sendMessage(['INIT'], { msg: 'test' })
            event.preventDefault();
        };
    };

    return (
        <div className="page">
            <header className="header">CarControl</header>
            <main className="main">
                <div>
                    <p>{car.Id}</p>
                </div>
                <div>
                    <p>{car.sensors.distance.left}</p>
                    <p>{car.sensors.distance.center}</p>
                    <p>{car.sensors.distance.right}</p>
                </div>
                <div></div>
                <button onClick={clickHandler()}>
                    test
                </button>
            </main>
            <footer className="footer">&copy; 2024 Vincent Brokx</footer>
        </div>
    );
}

export default App;
