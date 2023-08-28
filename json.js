{"source":"user_data", "option":"(login='priority2030' or password='priority2030') AND password='12345'"}

{"source" : "measurement", "option" : "user_id='1'"}

{"source" : "measurement", "target":["user_id", "time_stamp", "predict", "solar_power", "wind_power", "gen_power", "consumer1_power", "consumer2_power", "consumer3_power"], "value":[["2", "28-08-2023", "false", "264", "534", "354", "123", "435", "122"],["3", "28-08-2023", "false", "113", "134", "314", "123", "235", "222"]]}


{"source" : "measurement", "target":["predict", "solar_power"],"value":["false", "2"], "option": "user_id='1' AND time_stamp='24-08-2023'"}