
pi=3.14159;			// mmmm... pi
led_c=3000; 			// LED circumference or length of LED strip - 3 meters
led_r=led_c/pi/2; 	// radius of LED strip
section_l=5000/24; 	// length of section for printing
section_a=360/24; 	// angle of each section

* %translate([-80,-50,0]) cube([100,100,120]);

translate([-led_r,0,0]) rotate([90,0,0]) intersection() {
	translate([0,0,-50]) linear_extrude(height=100,convexity=10) difference() {
		square([led_r+20,led_r+20]);
		rotate(section_a) square([led_r*2,led_r*2]);
	}
	rotate(0.98) difference() {
		rotate_extrude($fn=360,convexity=10)  translate([led_r,0]) profile();
		for(i=[36:0.125:37]) rotate(i*10) {
			translate([led_r-58,0,0]) rotate([0,30,0]) 
				linear_extrude(height=50,convexity=10) square([4,12],center=true);
			translate([led_r-39,0,0]) rotate([0,-30,0]) 
				linear_extrude(height=50,convexity=10) square([4,12],center=true);
		}
		mirror([0,0,1]) for(i=[36:0.125:37]) rotate(i*10) {
			translate([led_r-58,0,0]) rotate([0,30,0]) 
				linear_extrude(height=50,convexity=10) square([4,12],center=true);
			translate([led_r-39,0,0]) rotate([0,-30,0]) 
				linear_extrude(height=50,convexity=10) square([4,12],center=true);
		}
	}
}

module profile() difference() {
	union() {
		square([8,32.25],center=true);
		difference() {
			intersection() {
				translate([-38,0]) circle(r=70,$fn=3);
				translate([-38,0]) rotate(60) circle(r=90,$fn=3);
			}
			intersection() {
				translate([-38,0]) circle(r=62,$fn=3);
				translate([-38,0]) rotate(60) circle(r=82,$fn=3);
			}
		}
	}
	square([4,16.5],center=true);
	translate([-2,0]) square([6,12.5],center=true);
	translate([-70,0]) square([20,46],center=true);
}
