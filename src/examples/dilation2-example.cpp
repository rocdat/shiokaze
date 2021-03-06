/*
**	dilation2-example.cpp
**
**	This is part of Shiokaze, a research-oriented fluid solver for computer graphics.
**	Created by Ryoichi Ando <rand@nii.ac.jp> on March 2, 2018.
**
**	Permission is hereby granted, free of charge, to any person obtaining a copy of
**	this software and associated documentation files (the "Software"), to deal in
**	the Software without restriction, including without limitation the rights to use,
**	copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
**	Software, and to permit persons to whom the Software is furnished to do so,
**	subject to the following conditions:
**
**	The above copyright notice and this permission notice shall be included in all copies
**	or substantial portions of the Software.
**
**	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
**	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
**	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
**	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
**	CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
**	OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//
#include <shiokaze/ui/drawable.h>
#include <shiokaze/core/console.h>
#include <shiokaze/array/array2.h>
#include <shiokaze/visualizer/gridvisualizer2_interface.h>
//
SHKZ_USING_NAMESPACE
//
class dilation2 : public drawable {
private:
	//
	LONG_NAME("Dilation 2D")
	ARGUMENT_NAME("DilationExample")
	//
	virtual void configure( configuration &config ) override {
		//
		config.get_unsigned("ResolutionX",shape[0],"Resolution towards X axis");
		config.get_unsigned("ResolutionY",shape[1],"Resolution towards Y axis");
		//
		double scale (1.0);
		config.get_double("ResolutionScale",scale,"Resolution doubling scale");
		shape *= scale;
		dx = shape.dx();
		//
		set_environment("shape",&shape);
		set_environment("dx",&dx);
	}
	//
	virtual void post_initialize() override {
		array.initialize(shape);
	}
	//
	virtual bool keyboard ( char key ) override {
		// Dump keyboard event
		console::dump( "Keyboard %c\n", key );
		if ( key == 'R' ) {
			reinitialize();
		} else if( key == 'C' ) {
			console::dump( "Count = %d\n", array.count());
		} else if( key == 'P') {
			auto &config = configurable::get_global_configuration();
			config.print_variables();
		}
		return true;
	}
	void fill( double x, double y ) {
		int i = shape[0] * x;
		int j = shape[0] * y;
		array.set(shape.clamp(i,j),1.0);
	}
	virtual void drag( int width, int height, double x, double y, double u, double v ) override {
		array.dilate( [&](int i, int j, auto& it) {
			it.set(1.0);
		});
	}
	virtual void mouse( int width, int height, double x, double y, int button, int action ) override {
		fill(x,y);
	}
	virtual void setup_window( std::string &name, int &width, int &height ) const override {
		double ratio = shape[1] / (double) shape[0];
		height = ratio * width;
	}
	virtual void draw( const graphics_engine &g, int width, int height ) const override {
		//
		// Draw things
		gridvisualizer->draw_grid(g);
		gridvisualizer->draw_density(g,array);
		//
		// Draw a message
		g.color4(1.0,1.0,1.0,1.0);
		g.draw_string(vec2d(0.025,0.025).v, "Press \"R\" to reset");
	}
	//
	array2<double> array {this};
	shape2 shape {42,32};
	double dx;
	gridvisualizer2_driver gridvisualizer{this,"gridvisualizer2"};
};
//
extern "C" module * create_instance() {
	return new dilation2;
}
//
extern "C" const char *license() {
	return "MIT";
}