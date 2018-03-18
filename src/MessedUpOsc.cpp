#include "RODENTMODULES.hpp"


struct MessedUpOsc : Module {
	enum ParamIds {
		PITCH_PARAM,
        PW_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		PITCH_INPUT,
        PW_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		SQUARE_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		BLINK_LIGHT,
		NUM_LIGHTS
	};

	float phase = 0.0;
	float blinkPhase = 0.0;

	MessedUpOsc() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;
};


void MessedUpOsc::step() {
	// Implement a simple sine oscillator
	float deltaTime = 1.0 / engineGetSampleRate();

	// Compute the frequency from the pitch parameter and input
	float pitch = params[PITCH_PARAM].value;
	pitch += inputs[PITCH_INPUT].value;
	pitch = clamp(pitch, -4.0f, 4.0f);
	float freq = 440.0 * powf(2.0, pitch);

	// Accumulate the phase
	phase += freq * deltaTime;
	if (phase >= 1.0)
		phase -= 1.0;
    
    //Mod param
    float pw = params[PW_PARAM].value*0.1+1;
    
    //Mod input
    float minput = inputs[PW_INPUT].value*0.3;
    
    //Mod param+input
    float pinput = (pw + minput);

	// Compute the sine output
	//float sine = sinf(2 * M_PI * phase);
	//outputs[SINE_OUTPUT].value = 5.0 * sine;
    
    //SQUARE shit
    float square = cos(exp(pinput * M_PI * phase));
    outputs[SQUARE_OUTPUT].value = 4 * square;

	// Blink light at 1Hz
	blinkPhase += deltaTime;
	if (blinkPhase >= 1.0)
		blinkPhase -= 1.0;
	lights[BLINK_LIGHT].value = (blinkPhase < 0.5) ? 1.0 : 0.0;
}


struct MessedUpOscWidget : ModuleWidget {
	MessedUpOscWidget(MessedUpOsc *module);
};


MessedUpOscWidget::MessedUpOscWidget(MessedUpOsc *module) : ModuleWidget(module) {

	/*
	box.size = Vec(208, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/MessedUpOsc.svg")));
		addChild(panel);
	}
	*/
	
	setPanel(SVG::load(assetPlugin(plugin, "res/MessedUpOsc.svg")));
	
	addChild(Widget::create<ScrewSilver>(Vec(15, 0)));
	addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 30, 0)));
	addChild(Widget::create<ScrewSilver>(Vec(15, 365)));
	addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 30, 365)));

	addParam(ParamWidget::create<Davies1900hWhiteKnob>(Vec(65, 230), module, MessedUpOsc::PITCH_PARAM, -3.0, 3.0, 0.0));
    addParam(ParamWidget::create<Davies1900hWhiteKnob>(Vec(24, 150), module, MessedUpOsc::PW_PARAM, -3.0, 3.0, 0.0));

	addInput(Port::create<PJ301MPort>(Vec(115, 236), Port::INPUT, module, MessedUpOsc::PITCH_INPUT));
    addInput(Port::create<PJ301MPort>(Vec(137, 152), Port::INPUT, module, MessedUpOsc::PW_INPUT));

	addOutput(Port::create<PJ301MPort>(Vec(90, 300), Port::OUTPUT, module, MessedUpOsc::SQUARE_OUTPUT));
}

Model *modelMessedUpOsc = Model::create<MessedUpOsc, MessedUpOscWidget>("RODENTMODULES", "MessedUpOsc", "Messed Up Oscillator", OSCILLATOR_TAG);