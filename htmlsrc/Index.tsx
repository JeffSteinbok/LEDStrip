import * as React from "react";
import * as ReactDOM from "react-dom";
import { WebServerApiManager, IApiExecutionContext } from "./utilities/WebServerApiManager";

interface ILEDState {
    effect: string;
}

export class LED extends React.Component<any, ILEDState> {

    globalEffectParams = {
        brightness: 16
    };

    perEffectParams = {
        "marquee": {},
        "comet":  {},
        "huerotate": {},
        "solid": {}
    };
    
    
    constructor(props: any) {
        super(props);

        this.state = {
            effect: "huerotate"
        }
    }

    render() {
        return (
            <div id="led">
                <select onChange={e => this.setEffect(e.target.value)}>
                    <option value="solid">Solid</option>
                    <option value="marquee">Marquee</option>
                    <option value="huerotate">Hue Rotate</option>
                    <option value="comet">Comet</option>
                </select>

                <div>
                    Brightness: <input onChange={e => { this.globalEffectParams.brightness = Number.parseInt(e.target.value); } } />
                </div>


                { this.state.effect == "solid" && 
                    <div>
                        Color (hex): <input onChange={e => { this.perEffectParams["solid"]["color"] = e.target.value; } } />
                    </div>
                }


                { this.state.effect == "comet" && 
                    <div>
                        Length: <input onChange={e => { this.perEffectParams["comet"]["length"] = e.target.value; } } />
                    </div>
                }

                <button onClick={e => { this.updateLeds() }}>Update</button>
            </div>
        );
    }

    setEffect = (effectName) => {
        this.setState({effect: effectName});
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
            apiName: "/effect?name=" + this.state.effect + qs,
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