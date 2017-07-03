	# .set UART_rx_irq,0x08
	# .set UART_tx_irq,0x10
	.set UART_tx_bfr_empty,0x40
	.equ U_rx_irq_en,0x20
	.equ U_tx_irq_en,0x40

	# save registers
	lui		$k0, %hi(_uart_buff)
	ori		$k0, $k0, %lo(_uart_buff)
	sw		$a0, 5*4($k0)
	sw		$a1, 6*4($k0)
	sw		$a2, 7*4($k0)
	sw		$a3, 8*4($k0)
	sw		$s0, 9*4($k0)

	lui		$a0, %hi(HW_uart_addr)  # get device's address
	ori		$a0, $a0, %lo(HW_uart_addr)
	nop
	jal		print
	nop

	lw		$k1, USTAT($k0) # Load status register
	# Se (STATS & tx_irq) == 0, então é interrupcao de RX
	andi	$a1, $k1, U_tx_irq
	beq		$a1, $zero, UARTrec
	nop
	j		UARTtra
	
	#---------------------------------------------------
	# handle reception
UARTrec:
	# Clear reception IRQ
	lw		$a1, UINTER($a0)
	ori		$a1, $a1, U_rx_irq
	sw		$a1, UINTER($a0)

	lw		$a1, UDATA($a0) 	# Read data from device
	lui		$a2, %hi(Ud)		# Load UART data address
	ori		$a2, $a2, %lo(Ud)
	lw		$a3, RXTL($a2)		# Get RX tail
	add		$s0, $a3, $a2		# Add it to Ud addr
	sw		$a1, RX_Q($s0)		# And then add the RX_Q offset
	addi	$a3, $a3, 1			# (rx_tail + 1) % Q_size
	andi	$a3, $a3, (Q_SZ - 1)
	sw		$a3, RXTL($a2)		# Store new RX_tail value

	# And, finally, increments NRX
	lw		$a3, NRX($a2)
	addi	$a3, $a3, 1
	sw		$a3, NRX($a2)
	
	lw		$a1, UINTER($a0)
	ori		$a1, $a1, U_rx_irq_en
	sw		$a1, UINTER($a0)

	j		_return
	nop

	
	
	#---------------------------------------------------
	# handle transmission
UARTtra:	
	lui		$a1, %hi(Ud)
	ori		$a1, $a1, %lo(Ud)
	lw		$a2, NTX($a2)
	addi	$a2, $a2, 1
	sw		$a2, NTX($a2)
	# your code goes here
	
	sw		$a1, UDATA($a0) 	# Write data to device


	#---------------------------------------------------
	# return	
	
_return:

	# restore registers
	lw		$a0, 5*4($k0)
	lw		$a1, 6*4($k0)
	lw		$a2, 7*4($k0)
	lw		$a3, 8*4($k0)
	lw		$s0, 9*4($k0)


	eret			    # Return from interrupt

