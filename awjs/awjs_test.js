


QUnit.test("test_conn_id", 
    function(assert) {
        assert.equal(ConnID.Slot, 'Slot');
        assert.equal(ConnID.Input, 'Input');
    }   
);


QUnit.test("test_ptype_id", 
    function(assert) {
        assert.equal(PTypeID.Trigger, 'Trigger');
        assert.equal(PTypeID.RateContext, 'RateContext');
    }   
);


QUnit.test("test_env_a", 
    function(assert) {
        var env1 = new Env(44100, 64);
        assert.ok(env1.get_nyquist() == 22050);

        var env2 = Env.prototype.get_default_env();
        assert.ok(env1.get_nyquist() == 22050);
    }   
);


QUnit.test("test_gen_resize_a", 
    function(assert) {
        var env = new Env(44100, 64);
        g1 = new Gen(env);
        g1.init();
        g1._resize_outputs();
        assert.equal(g1.outputs.length, 0);
    }   
);


QUnit.test("test_gen_register_output_parameter_type_a", 
    function(assert) {
        var env = new Env(44100, 64);
        g1 = new Gen(env);
        g1.init();
        // replace with PType instances 
        g1._register_input_parameter_type(PTypeID.Frequency);
        
        g1._register_output_parameter_type(PTypeID.Output);
        g1._register_output_parameter_type(PTypeID.Trigger);
        assert.equal(g1.outputs.length, 2);
        g1.reset();

        assert.equal(g1.outputs[0].length, env.frame_size);
        assert.equal(g1.outputs[1].length, env.frame_size);

        //window.alert(5 + 6);
    }   
);



