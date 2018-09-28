

function update_configure(c)

	local x = c:add_maps()
	
	local l
	l = x:add_locals()
	l:set_addr("0.0.0.0")
	l:set_port(6888)
	

	
	local r
	r = x:add_remotes()

	r:set_addr("172.105.196.90")
    --r:set_addr("127.0.0.1")
	r:set_port(2205)


    c:set_nb_services(4)

    c:set_run_as_daemon(0)                                                          --服务器后台运行 using both
end
