// -----------------------------------------------
// enumerations

var ConnID = Object.freeze({
    Slot: 'Slot',
    Input: 'Input',
    Output: 'Output',
});

var PTypeID = Object.freeze({
    Switch:'Switch',
    Value:'Value',
    Frequency:'Frequency',
    Duration:'Duration',
    Phase:'Phase',
    Channels:'Channels',
    Trigger:'Trigger',
    Cycle:'Cycle', // boolean for cycle control
    LowerBoundary:'LowerBoundary',
    UpperBoundary:'UpperBoundary',
    BreakPoints:'BreakPoints', // for slot in breakpoint, wavetable
    Buffer:'Buffer',
    Interpolation:'Interpolation',
    TimeContext:'TimeContext', // rename duration context?
    RateContext:'RateContext',
    Modulus:'Modulus', // for counter
    Direction:'Direction', //
    BoundaryContext:'BoundaryContext',
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
    this._env = env;

    // these are set from environment at each init
    this._frame_size = 0;
    this._sampling_rate = 0;
    this._nyquist = 0;

    this._output_count = 0;
    this._outputs_size = 0; // count * frame size

    // store a parameter type instnace by parameter index
    this._output_parameter_type = {};

    this._slot_count = 0;
    this._slots = []; // a list of Gens (do not declare which PIndex to read from, one per position)
    this._slot_parameter_type = {};

    this._input_count = 0;
    this._inputs = []; // a list of lists, where each inner list is a pair of Gen, PIndex
    this._input_parameter_type = {};

    this._render_count = 0;

    this._summed_inputs = [];
    // the output storage
    this.outputs = []; // a list of lists

};

// methods
Gen.prototype.init = function() {
    // base prototype init transfers frame zie
    this._frame_size = this._env.frame_size;
    this._sampling_rate = this._env.sampling_rate;
    this._nyquist = this._sampling_rate / 2;

};

Gen.prototype._resize_outputs = function() {
    this._outputs_size = this._output_count * this._frame_size;
    this.outputs = new Array(this._output_count);
    var i = 0;
    while (i < this._output_count) {
        this.outputs[i] = new Array(this._frame_size);
        i++;
    };
};

Gen.prototype.reset = function() {
    var i = 0;
    var j = 0;
    while (i < this._output_count) {
        while (j < this._frame_size) {
            this.outputs[i][j] = 0.0;
            j++;
        }
        i++;
        //window.alert('reset: i:' + i.toString() + ' j: ' + j.toString());
    }
    this._render_count = 0;

};

Gen.prototype._register_input_parameter_type = function(ptype) {
    this._input_parameter_type[this._input_count] = ptype;

    // the added array will store 1 or more pairs of Gen, PIndex
    this._inputs.push(new Array());

    // this needs to be common frame size, not just this Gen's frame size
    var vsum = new Array(this._frame_size);
    // TODO: initialize zeros
    this._summed_inputs.push(vsum);

    var set_index = this._input_count;
    this._input_count++;
    return set_index;
};


Gen.prototype._register_output_parameter_type = function(ptype) {
    this._output_parameter_type[this._output_count] = ptype;
    var set_index = this._output_count;
    this._output_count += 1;
    this._resize_outputs(); // note: called each time, leading to numerous allocs
    return set_index;
};


Gen.prototype._clear_output_parameter_types = function() {
    this._output_count = 0;
    this._outputs_size = 0;
    this.outputs = [];
};
