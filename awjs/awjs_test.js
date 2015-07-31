
QUnit.test("test_env_a", 
    function(assert) {
        var env1 = new Env(44100, 64);
        assert.ok(env1.get_nyquist() == 22050);

        var env2 = Env.prototype.get_default_environment();
        assert.ok(env1.get_nyquist() == 22050);
    }   
);


QUnit.test("test_gen_resize_a", 
    function(assert) {
        var env1 = new Env(44100, 64);
        g1 = new Gen(env1);
        g1._resize_outputs();
        assert.equal(g1.outputs.length, 1);
        assert.equal(g1.outputs[0].length, env1.frame_size);

        console.log(g1);
    }   
);



