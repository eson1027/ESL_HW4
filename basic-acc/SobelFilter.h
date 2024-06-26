#ifndef SOBEL_FILTER_H_
#define SOBEL_FILTER_H_
#include <cmath>
#include <iomanip>
#include <systemc>
using namespace sc_core;

#include <tlm_utils/simple_target_socket.h>

#include <tlm>

#include "filter_def.h"

struct SobelFilter : public sc_module {
	tlm_utils::simple_target_socket<SobelFilter> tsock;

	sc_fifo<unsigned char> i_r;
	sc_fifo<unsigned char> i_g;
	sc_fifo<unsigned char> i_b;
	sc_fifo<int> o_result;

	SC_HAS_PROCESS(SobelFilter);

	SobelFilter(sc_module_name n) : sc_module(n), tsock("t_skt"), base_offset(0) {
		tsock.register_b_transport(this, &SobelFilter::blocking_transport);
		SC_THREAD(do_filter);
	}

	~SobelFilter() {}

	unsigned int base_offset;

	void do_filter() {
		unsigned char buffer_gray[BUFFER_SIZE];
		{ wait(CLOCK_PERIOD, SC_NS); }
		// intialize the buffer
		for (int i = 0; i < BUFFER_SIZE; i++) {
			buffer_gray[i] = 0;
		}

		while (true) {
			for (uint_fast16_t i = 24; i >= 5; i--) {
				buffer_gray[i] = buffer_gray[i - 5];
			}

			for (uint_fast16_t i = 0; i < 5; i++) {
				buffer_gray[i] = (i_r.read() + i_g.read() + i_b.read()) / 3;
			}

			uint_fast32_t val = buffer_gray[0] * 1 + buffer_gray[1] * 4 + buffer_gray[2] * 7 + buffer_gray[3] * 4 +
			                    buffer_gray[4] * 1 + buffer_gray[5] * 4 + buffer_gray[6] * 16 + buffer_gray[7] * 26 +
			                    buffer_gray[8] * 16 + buffer_gray[9] * 4 + buffer_gray[10] * 7 + buffer_gray[11] * 26 +
			                    buffer_gray[12] * 41 + buffer_gray[13] * 26 + buffer_gray[14] * 7 +
			                    buffer_gray[15] * 4 + buffer_gray[16] * 16 + buffer_gray[17] * 26 +
			                    buffer_gray[18] * 16 + buffer_gray[19] * 4 + buffer_gray[20] * 1 + buffer_gray[21] * 4 +
			                    buffer_gray[22] * 7 + buffer_gray[23] * 4 + buffer_gray[24] * 1;

			// cout << (int)result << endl;

			o_result.write(val);
			// wait(29000*CLOCK_PERIOD, SC_NS);
		}
	}
	void blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay) {
		delay+=sc_time(29*CLOCK_PERIOD, SC_NS);
		// wait(delay);
		// wait(29 * CLOCK_PERIOD, SC_NS);
		// unsigned char *mask_ptr = payload.get_byte_enable_ptr();
		// auto len = payload.get_data_length();
		tlm::tlm_command cmd = payload.get_command();
		sc_dt::uint64 addr = payload.get_address();
		unsigned char *data_ptr = payload.get_data_ptr();

		addr -= base_offset;

		// cout << (int)data_ptr[0] << endl;
		// cout << (int)data_ptr[1] << endl;
		// cout << (int)data_ptr[2] << endl;
		word buffer;

		switch (cmd) {
			case tlm::TLM_READ_COMMAND: {
				// cout << "READ" << endl;
				// int k = o_result.num_available();
				switch (addr) {
					case SOBEL_FILTER_RESULT_ADDR: {
						buffer.uint = o_result.read();

						// std::cout << int(k) << std::endl;
						break;
					}
					default: {
						std::cerr << "READ Error! SobelFilter::blocking_transport: address 0x" << std::setfill('0')
						          << std::setw(8) << std::hex << addr << std::dec << " is not valid" << std::endl;
					}
				}
				// data_ptr[0] = buffer.uc[0];
				// data_ptr[1] = buffer.uc[1];
				// data_ptr[2] = buffer.uc[2];
				// data_ptr[3] = buffer.uc[3];
				data_ptr[0] = buffer.uint;
				data_ptr[1] = buffer.uint >> 8;
				data_ptr[2] = buffer.uint >> 16;
				data_ptr[3] = buffer.uint >> 24;
				break;
			}
			case tlm::TLM_WRITE_COMMAND: {
				// cout << "WRITE" << endl;
				switch (addr) {
					case SOBEL_FILTER_R_ADDR: {
						i_r.write(data_ptr[0]);
						i_g.write(data_ptr[1]);
						i_b.write(data_ptr[2]);
						break;
					}
					default: {
						std::cerr << "WRITE Error! SobelFilter::blocking_transport: address 0x" << std::setfill('0')
						          << std::setw(8) << std::hex << addr << std::dec << " is not valid" << std::endl;
					}
				}
				break;
				case tlm::TLM_IGNORE_COMMAND: {
					payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
					return;
				}
				default: {
					payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
					return;
				}
			}
		}
		payload.set_response_status(tlm::TLM_OK_RESPONSE);  // Always OK
	}
};
#endif
