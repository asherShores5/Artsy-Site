(module
	;; WAT Setup Declarations
	(import "env" "jsprint" (func $jsprint (param i32)))
	(import "env" "newline" (func $newline))
	(import "env" "writeconsoleInt" (func $writeconsoleInt (param i32)))
	(import "env" "writeconsoleFloat" (func $writeconsoleFloat (param f32)))
	(import "env" "writeconsoleString" (func $writeconsoleString (param i32)))
	(memory $0 100)
	(export "pagememory" (memory $0))
	(func $create_array (param $size i32) (result i32)
		(local $ptr i32)
		(set_local $ptr (i32.const 0))
		(block
			(loop $loop
				(br_if $loop (i32.eq (get_local $size) (i32.const 0)))
				(set_local $ptr (i32.add (get_local $ptr) (i32.const 4)))
				(set_local $size (i32.sub (get_local $size) (i32.const 1)))
			)
		)
		(get_local $ptr)
	)
	(export "create_array" (func $get_element))
	(func $get_element (param $ptr i32) (param $index i32) (result i32)
		(i32.load (i32.add (get_local $ptr) (i32.mul (get_local $index) (i32.const 4))))
	)
	(export "get_element" (func $get_element))
	(func $set_element (param $ptr i32) (param $index i32) (param $value i32)
		(i32.store (i32.add (get_local $ptr) (i32.mul (get_local $index) (i32.const 4))) (get_local $value))
	)
	(export "set_element" (func $set_element))

	;; Artsy Program in WAT
	(global $x (mut f32) (f32.const 0.0))
	;; Start Main Function
	(func $main

		(global.set $x
			(f32.const 10.0)
		)
		(call $writeconsoleFloat (global.get $x))
	)
	(start $main)
)
