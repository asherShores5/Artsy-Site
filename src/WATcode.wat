(module
	;; WAT Setup Declarations
	(import "env" "print_int" (func $print_int (param i32)))
	(import "env" "print_float" (func $print_float (param f32)))
	(import "env" "print_string" (func $print_string (param i32)))
	(import "env" "newline" (func $newline))
	(memory $0 100)
	(export "pagememory" (memory $0))
	(func $create_array (param $size i32) (result i32) (local $ptr i32) (set_local $ptr (i32.const 0)) (block (loop $loop (br_if $loop (i32.eq (get_local $size) (i32.const 0))) (set_local $ptr (i32.add (get_local $ptr) (i32.const 4))) (set_local $size (i32.sub (get_local $size) (i32.const 1))))) (get_local $ptr))
	(export "create_array" (func $create_array))
	(func $get_element_i32 (param $ptr i32) (param $index i32) (result i32) (i32.load (i32.add (get_local $ptr) (i32.mul (get_local $index) (i32.const 4)))))
	(export "get_element_i32" (func $get_element_i32))
	(func $set_element_i32 (param $ptr i32) (param $index i32) (param $value i32) (i32.store (i32.add (get_local $ptr) (i32.mul (get_local $index) (i32.const 4))) (get_local $value)))
	(export "set_element_i32" (func $set_element_i32))

	(func $get_element_f32 (param $ptr i32) (param $index i32) (result f32) (f32.load (i32.add (get_local $ptr) (i32.mul (get_local $index) (i32.const 4)))))
	(export "get_element_f32" (func $get_element_f32))
	(func $set_element_f32 (param $ptr i32) (param $index i32) (param $value f32) (f32.store (i32.add (get_local $ptr) (i32.mul (get_local $index) (i32.const 4))) (get_local $value)))
	(export "set_element_f32" (func $set_element_f32))

	;; Artsy Program in WAT
	(global $x (mut i32) (i32.const 0))
	(global $y (mut i32) (i32.const 0))
	(func $test (param $a i32) (param $b i32) (result i32)
		(local $T0 i32)
		(local $T1 i32)
		(local $T2 i32)
		(local $T3 i32)
		(local $T4 i32)
		(local.set $T0
		(i32.div_s		(local.get $a)		(local.get $b)		))
		(local.set $T1
		(i32.mul		(local.get $T0)		(local.get $a)		))
		(local.set $T2
		(i32.div_s		(local.get $a)		(local.get $b)		))
		(local.set $T3
		(i32.mul		(local.get $T2)		(local.get $a)		))
		(local.set $T4
		(i32.div_s		(local.get $T3)		(local.get $b)		))
		(return		(local.get $T4))
		
	)
	(export "test" (func $test))
	;; Start Main Function
	(func $main
		(local $T5 i32)

		(global.set $y
 (i32.const 84))
		(local.set $T5 (call $test
		(global.get $y) (i32.const 3)))
		(global.set $x
		(local.get $T5))
		(call $print_int		(global.get $y))
		(call $newline)
		(call $print_int		(global.get $x))
	)
	(start $main)
)
