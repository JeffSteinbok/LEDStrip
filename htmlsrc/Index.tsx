import * as React from "react";
import * as ReactDOM from "react-dom";
import { WebServerApiManager, IApiExecutionContext } from "./utilities/WebServerApiManager";
import { HexColorPicker } from "react-colorful";
import "react-colorful/dist/index.css";

interface ILEDState {
    effect: string;
    colorValue: string;
}

export class LED extends React.Component<any, ILEDState> {

    globalEffectParams = {
        brightness: 16
    };

    perEffectParams = {
        "marquee": {},
        "comet": {},
        "huerotate": {},
        "solid": {
            "color": "FF0000"
        }
    };


    constructor(props: any) {
        super(props);

        this.state = {
            effect: "solid",
            colorValue: "#ddd"
        }
    }

    render() {
        return (
            <div id="main">
                <h1>LED Strip</h1>
                <div id="configSection">
                    <div className="config">
                        <div className="name">Effect: </div>
                        <div>
                            <select onChange={ e => this.setEffect(e.target.value) }>
                                <option value="solid">Solid</option>
                                <option value="marquee">Marquee</option>
                                <option value="huerotate">Hue Rotate</option>
                                <option value="comet">Comet</option>
                                <option value="twinkle">Twinkle</option>
                            </select>
                        </div>
                    </div>

                    <div className="config">
                        <div className="name">Brightness:</div>
                        <div>
                            <input defaultValue={ 16 }
                                onChange={ e => { this.globalEffectParams.brightness = Number.parseInt(e.target.value); } } />
                        </div>
                    </div>


                    { this.state.effect == "solid" &&
                        <div className="config">
                            <div className="name"> Color (hex):</div>
                            <div>
                                <HexColorPicker color={ this.state.colorValue }
                                    onChange={ color => {
                                        this.setState({ colorValue: color });
                                        this.perEffectParams["solid"]["color"] = color.substring(1);
                                    } } />
                                <input onChange={ e => {
                                    this.setState({ colorValue: e.target.value });
                                    this.perEffectParams["solid"]["color"] = e.target.value.substring(1);
                                } }
                                    value={ this.state.colorValue } />
                            </div>

                        </div>
                    }


                    { this.state.effect == "comet" &&
                        <div className="config">
                            <div className="name">Length:</div>
                            <div>
                                <input defaultValue={ 10 }
                                    onChange={ e => { this.perEffectParams["comet"]["length"] = e.target.value; } } />
                            </div>
                        </div>
                    }
                </div>
                <p />
                <button onClick={ e => { this.updateLeds() } }>Update LED Strip</button>
            </div>
        );
    }

    setEffect = (effectName) => {
        this.setState({ effect: effectName });
    }

    updateLeds = () => {
        let qs = "";
        let perEffectParams = this.perEffectParams[this.state.effect];

        for (var key in this.globalEffectParams) {
            if (this.globalEffectParams.hasOwnProperty(key)) {
                qs += "&" + key + "=" + this.globalEffectParams[key] + "&";
            }
        }

        for (var key in perEffectParams) {
            if (perEffectParams.hasOwnProperty(key)) {
                qs += "&" + key + "=" + perEffectParams[key] + "&";
            }
        }

        let context = {
            showProgressIndicator: true,
            apiName: "http://192.168.2.14/effect?name=" + this.state.effect + qs,
            json: false,
            formData: null,
            success: null,
            error: null
        };

        let wsam = new WebServerApiManager();
        wsam.executeApi(context);
    }
}


// Start the application
let root = document.createElement("div");
root.id = 'main';
document.body.appendChild(root);
ReactDOM.render(
    <LED />,
    document.getElementById("main")
);
