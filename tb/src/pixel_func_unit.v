`include "config.vh"
reg [31:0] memory_data['MEMOEY_SIZE-1:0];

task init_memory

	initial	$readmemb(BINFILE, memory_data);

endtask


module feed_pixel_rgb888
(
input                           clk,
input                           rst,
input							valid_i,
reg [32-1:0:0]					addr,
output [8-1:0]					data_r,
output [8-1:0]					data_g,
output [8-1:0]					data_b,
);


reg [32-1:0:0]			rdata0;
reg [32-1:0:0]			rdata1;
reg [32-1:0:0]			rdata2;
reg [32-1:0:0]			rdata3;
reg [2-1:0]				addr_count_r;
reg [2-1:0]				addr_count_nxt_c;

assign addr_count_nxt_c = (addr_count_r==2'b11)?2'b0:(addr_count_r+1'b1);

always(posedge clk)
begin
	if(rst)
	begin
		addr_count_r  <=	2'b0;
	end
	else if(valid_i)
		addr_count_r <= addr_count_nxt_c;
end

always *
begin
	if(valid!=1'b1)
	begin
		data_r = 'dx;
		data_g = 'dx;
		data_b = 'dx;
		addr_count = 2'b0;
	end
	else
	begin
		if(addr_count<2'b11)
		begin
			if(addr_count==2'b0)
			begin
				data_r = rdata0[7:0];
				data_g = rdata0[15:8];
				data_b = rdata0[23:16];
			end
			else if(addr_count==2'b1)
			begin
				data_r = rdata0[31:24];
				data_g = rdata1[7:0];
				data_b = rdata1[15:8];
			end
			else if(addr_count==2'b10)
			begin
				data_r = rdata1[23:16];
				data_g = rdata1[31:24];
				data_b = rdata2[7:0];
			end
			else
			begin
				data_r = rdata2[15:8];
				data_g = rdata2[23:16];
				data_b = rdata2[31:24];
			end
		end
		else
		begin
			rdata0=memory_data[addr];
			rdata1=memory_data[addr+1'b1];
			rdata2=memory_data[addr+2'b10];
			addr=addr+'d3;
		end	
	end
end

endmodule

module output_monitor_ppm
(
input                           clk,
input                           rst,
input							valid_i,
input ['PIXEL_DEPTH-1:0]		data_r_i,
input ['PIXEL_DEPTH-1:0]		data_g_i,
input ['PIXEL_DEPTH-1:0]		data_b_i
);

reg 							valid_r;
reg ['PIXEL_DEPTH-1:0]			data_r_r;
reg ['PIXEL_DEPTH-1:0]			data_g_r;
reg ['PIXEL_DEPTH-1:0]			data_b_r;

reg								frame_start_r;

always(posedge clk)
begin
	if(rst)
	begin
		valid_r  <=	1'b0;
	end
	else
		valid_r<=valid_i;
end

always(posedge clk)
begin
if(rst)
begin
	data_r_r < = 'd0;
	data_g_r < = 'd0;
	data_b_r < = 'd0;
end
else if(valid_i)
	data_r_r < = data_r_i;
	data_g_r < = data_g_i;
	data_b_r < = data_b_i;
end

always(valid_i&(~valid_r))
begin
	fp=$fopen(filename,'w');
	$fdisplay(fp,"P3");
	$fdisplay(fp,"%d %d",'IMG_WIDTH,'IMG_HEIGHT);
	$fdisplay(fp,"%d",'COLOR_DEPTH);
	frame_start_r=1;
	while(frame_start_r)
	begin
		@(posedge clk);
		begin
			pixel_cnt=pixel_cnt+1'b1;
			if(pixel_cnt=='IMG_WIDTH)
			begin
				$fdisplay(fp,"%d %d %d",data_r_r,data_g_r,data_b_r);
				pixel_cnt = 'd0;
				line_cnt=line_cnt+1'b1;
				if(line_cnt=='IMG_HEIGHT)
				begin
					frame_start_r = 1'b0;
				end
			end
			else
			begin
				$fwrite(fp,"%d %d %d ",data_r_r,data_g_r,data_b_r);
			end
			
		end
	end
	$fclose(fp);
end


endmodule