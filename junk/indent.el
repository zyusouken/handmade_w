;;When tab is pressed
(if nil ;;Evaluate if selection exists
	;;If selection exists
	(progn
		(message "YES!")
		(message "SELECTION!")
		;; Move caret up one line
		;; (to capture the break from
		;; the start of selection)

		;;then indent rigidly RIGHT 1 tab
	)
	;;Else (NO selection)
	(progn
		(message "no...")
		(message "selection...")
		;; just insert a tab. lol
		(insert "\t")
	)
)
