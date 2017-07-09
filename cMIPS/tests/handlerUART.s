	
	# .set UART_rx_irq,0x08
	# .set UART_tx_irq,0x10
	.set UART_tx_bfr_empty,0x40

	lui		$k0, %hi(_uart_buff)
	ori		$k0, $k0, %lo(_uart_buff)
	# save registers
	sw		$s0,  5*4($k0)
	sw		$s1,  6*4($k0)
	sw		$s2,  7*4($k0)
	sw		$s3,  8*4($k0)
	sw		$s4,  9*4($k0)
	sw		$s5, 10*4($k0)
	sw		$a0, 11*4($k0)


	# replace $xx for the apropriate registers
	lui		$k1, %hi(HW_uart_addr)  # get device's address
	ori		$k1, $k1, %lo(HW_uart_addr)
	
	# your code goes here
	lui		$a0, %hi(Ud)
	ori		$a0, $a0, %lo(Ud)
	lw		$s0, USTAT($k1)
	andi	$s0, $s0, U_rx_irq
	beq		$s0, $zero, UARTtra
	nop
	#jal		print
	j		UARTrec
	#j		_return_uart
	
	#---------------------------------------------------
	# handle reception
UARTrec:
	lb		$s0, UDATA($k1) 	# Read data from device

	# your code goes here
	lw		$s1, RXTL($a0)
	add		$s2, $s1, $a0
	sb		$s0, RX_Q($s2)
	addi	$s1, $s1, 1
	andi	$s1, $s1, (Q_SZ - 1)
	sw		$s1, RXTL($a0)
	lw		$s1, NRX($a0)
	addi	$s1, $s1, 1
	sw		$s1, NRX($a0)

	lw		$s1, UINTER($k1)
	ori		$s1, $s1, U_rx_irq
	sw		$s1, UINTER($k1)

	nop
	
	j		_return_uart
	nop
	
	#---------------------------------------------------
	# handle transmission
UARTtra:	
	# If there's nothing in the queue waiting to be transmitted
	# just unset the tx started flag and return.
	jal		print
	lw		$s1, NTX($a0)
	beqz	$s1, stop_tx
	nop

	jal		print
	nop

	# Otherwise do the tx normally.
	# Do some weird ASM math to get to Ud.rx_q[Ud.tx_hd]
	lw		$s2, TXHD($a0)
	add		$s3, $s2, $a0
	lb		$s0, TX_Q($s3)

	# Decrements NTX and increments TX_HD (mod Q_SZ).
	addi	$s1, $s1, -1
	sw		$s1, NTX($a0)
	addi	$s2, $s2, 1
	andi	$s2, $s2, (Q_SZ - 1)
	sw		$s2, RXHD($a0)

	# Last, but not least important, clearing the damned interruption.
	lw		$s1, UINTER($k1)
	ori		$s1, $s1, U_rx_irq
	sw		$s1, UINTER($k1)

	# your code goes here
	sw		$s0, UDATA($k1) 	# Read data from device
	j		_return_uart
	nop

stop_tx:
	# Turn off the TX interruption to avoid hanging the processor.
	lw		$s1, UINTER($k1)
	ori		$s1, $s1, 0x1 # Just keeps the RX part of it.
	sw		$s1, UINTER($k1)

	# Also turn off the TX started flag and return.
	lui		$s1, 0x0
	lui		$a0, %hi(tx_has_started)
	ori		$a0, $a0, %lo(tx_has_started)
	sw		$s1, 0($a0)

	lui		$a0, 0xDEAD
	ori		$a0, $a0, 0xDEAD
	jal		print
	nop

	#---------------------------------------------------
	# return	
	
_return_uart:

	# restore registers
	lw		$s0,  5*4($k0)
	lw		$s1,  6*4($k0)
	lw		$s2,  7*4($k0)
	lw		$s3,  8*4($k0)
	lw		$s4,  9*4($k0)
	lw		$s5, 10*4($k0)
	lw		$a0, 11*4($k0)

	eret			    # Return from interrupt

