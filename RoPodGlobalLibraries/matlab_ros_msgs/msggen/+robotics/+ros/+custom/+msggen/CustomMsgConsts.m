classdef CustomMsgConsts
    %CustomMsgConsts This class stores all message types
    %   The message types are constant properties, which in turn resolve
    %   to the strings of the actual types.
    
    %   Copyright 2014-2018 The MathWorks, Inc.
    
    properties (Constant)
        ropod_ros_msgs_ropod_control_primitive = 'ropod_ros_msgs/ropod_control_primitive'
        ropod_ros_msgs_ropod_demo_area = 'ropod_ros_msgs/ropod_demo_area'
        ropod_ros_msgs_ropod_demo_location = 'ropod_ros_msgs/ropod_demo_location'
        ropod_ros_msgs_ropod_demo_plan = 'ropod_ros_msgs/ropod_demo_plan'
        ropod_ros_msgs_ropod_demo_sem_location = 'ropod_ros_msgs/ropod_demo_sem_location'
        ropod_ros_msgs_ropod_demo_sem_location_list = 'ropod_ros_msgs/ropod_demo_sem_location_list'
        ropod_ros_msgs_ropod_demo_status = 'ropod_ros_msgs/ropod_demo_status'
        ropod_ros_msgs_ropod_demo_status_update = 'ropod_ros_msgs/ropod_demo_status_update'
        ropod_ros_msgs_ropod_demo_waypoint = 'ropod_ros_msgs/ropod_demo_waypoint'
        ropod_ros_msgs_ropod_door_detection = 'ropod_ros_msgs/ropod_door_detection'
        ropod_ros_msgs_ropod_sem_waypoint = 'ropod_ros_msgs/ropod_sem_waypoint'
        ropod_ros_msgs_ropod_sem_waypoint_list = 'ropod_ros_msgs/ropod_sem_waypoint_list'
        ropod_ros_msgs_sem_waypoint_cmd = 'ropod_ros_msgs/sem_waypoint_cmd'
    end
    
    methods (Static, Hidden)
        function messageList = getMessageList
            %getMessageList Generate a cell array with all message types.
            %   The list will be sorted alphabetically.
            
            persistent msgList
            if isempty(msgList)
                msgList = cell(13, 1);
                msgList{1} = 'ropod_ros_msgs/ropod_control_primitive';
                msgList{2} = 'ropod_ros_msgs/ropod_demo_area';
                msgList{3} = 'ropod_ros_msgs/ropod_demo_location';
                msgList{4} = 'ropod_ros_msgs/ropod_demo_plan';
                msgList{5} = 'ropod_ros_msgs/ropod_demo_sem_location';
                msgList{6} = 'ropod_ros_msgs/ropod_demo_sem_location_list';
                msgList{7} = 'ropod_ros_msgs/ropod_demo_status';
                msgList{8} = 'ropod_ros_msgs/ropod_demo_status_update';
                msgList{9} = 'ropod_ros_msgs/ropod_demo_waypoint';
                msgList{10} = 'ropod_ros_msgs/ropod_door_detection';
                msgList{11} = 'ropod_ros_msgs/ropod_sem_waypoint';
                msgList{12} = 'ropod_ros_msgs/ropod_sem_waypoint_list';
                msgList{13} = 'ropod_ros_msgs/sem_waypoint_cmd';
            end
            
            messageList = msgList;
        end
        
        function serviceList = getServiceList
            %getServiceList Generate a cell array with all service types.
            %   The list will be sorted alphabetically.
            
            persistent svcList
            if isempty(svcList)
                svcList = cell(0, 1);
            end
            
            % The message list was already sorted, so don't need to sort
            % again.
            serviceList = svcList;
        end
        
        function actionList = getActionList
            %getActionList Generate a cell array with all action types.
            %   The list will be sorted alphabetically.
            
            persistent actList
            if isempty(actList)
                actList = cell(0, 1);
            end
            
            % The message list was already sorted, so don't need to sort
            % again.
            actionList = actList;
        end
    end
end
