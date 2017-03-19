
pi=3.14159;				    // mmmm... pi
led_c=5000; 				// LED circumference or length of LED strip - 3 meters
led_r=led_c/pi/2; 		    // radius of LED strip
sections=45;				// circumference / maximum build height = min number of sections 
                            // then add a little to equal even number of degrees
section_l=5000/sections; 	// length of section for printing
section_a=360/sections; 	// angle of each section

print_section=false;        // change to true to see printable section

translate([-led_r,0,0]) rotate([90,0,0]) intersection() {
	if(print_section) translate([0,0,-50]) linear_extrude(height=100,convexity=10) 
        difference() {
		square([led_r+20,led_r+20]);
		rotate(section_a) square([led_r*2,led_r*2]);
	}
	color("Gainsboro")  /*rotate(120) rotate([0,45,0])*/ union() {
		rotate_extrude($fn=360,convexity=10)  translate([led_r,0]) profile();
		translate([led_r+6,0,0]) rotate([0,90,0]) gusset();
		// change from 360 to 10 for render
		for(i=[0:section_a:360]) rotate(i) { 
			translate([led_r+6,0,0]) rotate([0,90,0]) gusset();
			translate([led_r-38,0,0]) rotate([0,210,0]) translate([0,0,45]) gusset();
			translate([led_r-38,0,0]) rotate([0,330,0]) translate([0,0,44]) gusset();
		}
	}
}

module profile() difference() {
	union() {
		intersection() {
			translate([-38,0]) circle(r=64,$fn=3);
			square([9,40],center=true);
		}
		difference() {
			intersection() {
				translate([-38,0]) circle(r=70,$fn=3);
				translate([-38,0]) rotate(60) circle(r=90,$fn=3);
			}
			intersection() {
				translate([-38,0]) circle(r=64,$fn=3);
				translate([-38,0]) rotate(60) circle(r=84,$fn=3);
			}
		}
	}
	square([5,18],center=true);
	translate([-2,0]) square([6,8],center=true);
	translate([-70,0]) square([20,46],center=true);
}

module gusset() difference(){
	translate([-5,-15,0]) cube([10,30,9.5]);
	translate([0,2.5,5]) rotate([90,0,0]) cylinder(r=3.5/2,h=14,$fn=32,center=true);
	//%translate([0,-2,5]) rotate([90,0,0]) cylinder(r=3/2,h=14,$fn=32,center=true);
	translate([0,0,5])  translate([0,-5,0]) rotate([90,0,0]) cylinder(r=6/2,h=10,$fn=32);
	translate([0,0,5])  translate([0,5,0]) rotate([-90,0,0]) cylinder(r=6/sqrt(3),h=10,$fn=6);
	translate([-6,-15,0]) rotate([45,0,0]) cube([12,20,20]);
	translate([-6,15,0]) rotate([45,0,0]) cube([12,20,20]);
}

