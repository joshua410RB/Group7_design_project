`timescale 1 ns/10 ps  // time-unit = 1 ns, precision = 10 ps


module erosion_filter_tb(
);
    reg clk;
    reg rst_n;
    reg i_pixel;
    wire o_convolved_data;
	 wire tap0;
	 wire tap1;
	 wire pixelEnable_check;
 	 wire [8:0] pixelCounter1;
 	 wire [3:0] pixelRowCounter1;
	 wire [4:0] pixelStartUpCounter_check;
	 
    reg i_valid;
//	 wire o_valid;
    localparam period = 20; 
	 integer i;

    // Clock Generation
    always 
    begin
        clk = 1'b1; 
        #20; // high for 20 * timescale = 20 ns

        clk = 1'b0;
        #20; // low for 20 * timescale = 20 ns
    end

    // Testing
    initial begin
	 	  rst_n = 1'b0;
        i_valid = 1'b0;
		  i_pixel = 0;
		  
		  @(posedge clk)
		  rst_n = 1'b1;
        i_valid = 1'b1;
		  #1;
		  
        
		  // 000000000
		  // 000011000
		  // 001100000
		  // 011110000
		  // 011110010
		  // 001100000
		  
			// First Row
		  for (i = 0; i < 9; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 0;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end
			
			// Second Row
		  for (i = 0; i < 4; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 0;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end
		  for (i = 0; i < 2; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 1;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end
		  for (i = 0; i < 3; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 0;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end
		  // Third Row
		  for (i = 0; i < 2; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 0;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end
		  for (i = 0; i < 2; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 1;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end
		  for (i = 0; i < 5; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 0;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end
			// Fourth Row
		  for (i = 0; i < 1; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 0;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end		  
		  for (i = 0; i < 4; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 1;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end		  
		  		  for (i = 0; i < 4; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 0;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end		 
			// Fifth Row
		  for (i = 0; i < 1; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 0;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end		  
		  for (i = 0; i < 4; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 1;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end		  
		  for (i = 0; i < 2; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 0;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end	
		  for (i = 0; i < 1; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 1;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end	
		  for (i = 0; i < 1; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 0;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end	
		 	// Sixth Row
		  for (i = 0; i < 2; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 0;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end		  
		  for (i = 0; i < 2; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 1;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end		  
		  for (i = 0; i < 5; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 0;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end	 
		  for (i = 0; i < 36; i=i+1)
		  begin
		      @(posedge clk)
				#1;
				i_pixel = 0;
				#2;
            $display("Convolved Data Output=%d", o_convolved_data);

		  end	
        $finish;
    end

		convolution3x3_erosion filter(
			.clk(clk),
			.rst_n(rst_n),
			.i_pixel(i_pixel),
			.i_pixel_valid(i_pixel_valid),
			.o_pixel(o_convolved_data),
			.pixelCounter1(pixelCounter1),
 	      .pixelRowCounter1(pixelRowCounter1),
			.pixelStartUpCounter_check(pixelStartUpCounter_check),
			.pixelEnable_check(pixelEnable_check),
		   .tap0(tap0),
		   .tap1(tap1)
			
		);
		
endmodule