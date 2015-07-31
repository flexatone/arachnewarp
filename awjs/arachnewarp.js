// -----------------------------------------------
// enumerations

var ConnID = Object.freeze({
    Slot: 'Slot',
    Input: 'Input',
    Output: 'Output',
});

// -----------------------------------------------
var Env = function(sampling_rate, frame_size) {
    this.sampling_rate = sampling_rate;
    this.frame_size = frame_size;
};
// constructors
Env.prototype.get_default_environment = function() {
    return new Env(44100, 32);
};
// methods
Env.prototype.get_nyquist = function() {
    return this.sampling_rate * .5;
};

// -----------------------------------------------
var Gen = function(env) {
    this.env = env;
    this.output_count = 1;
    this.output_size = 0; // count * frame size
    this.outputs = []; // a list of lists
};

// methods
Gen.prototype._resize_outputs = function() {
    this.output_size = this.output_count * this.env.frame_size;
    this.outputs = new Array(this.output_count);
    for (i = 0; i < this.output_count; i++) {
        this.outputs[i] = new Array(this.env.frame_size);
    };
};

Gen.prototype._register_input_parameter_type = function() {
}
Gen.prototype._register_output_parameter_type = function() {
}



