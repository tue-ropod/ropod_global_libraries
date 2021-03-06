classdef ropod_demo_status_update < robotics.ros.Message
    %ropod_demo_status_update MATLAB implementation of ropod_ros_msgs/ropod_demo_status_update
    %   This class was automatically generated by
    %   robotics.ros.msg.internal.gen.MessageClassGenerator.
    
    %   Copyright 2014-2018 The MathWorks, Inc.
    
    %#ok<*INUSD>
    
    properties (Constant)
        MessageType = 'ropod_ros_msgs/ropod_demo_status_update' % The ROS message type
    end
    
    properties (Constant, Hidden)
        MD5Checksum = '6cd96983e359f849f57cac91271f1a44' % The MD5 Checksum of the message definition
    end
    
    properties (Access = protected)
        JavaMessage % The Java message object
    end
    
    properties (Constant, Access = protected)
        RopodRosMsgsRopodDemoStatusClass = robotics.ros.msg.internal.MessageFactory.getClassForType('ropod_ros_msgs/ropod_demo_status') % Dispatch to MATLAB class for message type ropod_ros_msgs/ropod_demo_status
    end
    
    properties (Dependent)
        Id
        Status
    end
    
    properties (Access = protected)
        Cache = struct('Status', []) % The cache for fast data access
    end
    
    properties (Constant, Hidden)
        PropertyList = {'Id', 'Status'} % List of non-constant message properties
        ROSPropertyList = {'id', 'status'} % List of non-constant ROS message properties
    end
    
    methods
        function obj = ropod_demo_status_update(msg)
            %ropod_demo_status_update Construct the message object ropod_demo_status_update
            import com.mathworks.toolbox.robotics.ros.message.MessageInfo;
            
            % Support default constructor
            if nargin == 0
                obj.JavaMessage = obj.createNewJavaMessage;
                return;
            end
            
            % Construct appropriate empty array
            if isempty(msg)
                obj = obj.empty(0,1);
                return;
            end
            
            % Make scalar construction fast
            if isscalar(msg)
                % Check for correct input class
                if ~MessageInfo.compareTypes(msg(1), obj.MessageType)
                    error(message('robotics:ros:message:NoTypeMatch', obj.MessageType, ...
                        char(MessageInfo.getType(msg(1))) ));
                end
                obj.JavaMessage = msg(1);
                return;
            end
            
            % Check that this is a vector of scalar messages. Since this
            % is an object array, use arrayfun to verify.
            if ~all(arrayfun(@isscalar, msg))
                error(message('robotics:ros:message:MessageArraySizeError'));
            end
            
            % Check that all messages in the array have the correct type
            if ~all(arrayfun(@(x) MessageInfo.compareTypes(x, obj.MessageType), msg))
                error(message('robotics:ros:message:NoTypeMatchArray', obj.MessageType));
            end
            
            % Construct array of objects if necessary
            objType = class(obj);
            for i = 1:length(msg)
                obj(i,1) = feval(objType, msg(i)); %#ok<AGROW>
            end
        end
        
        function id = get.Id(obj)
            %get.Id Get the value for property Id
            id = char(obj.JavaMessage.getId);
        end
        
        function set.Id(obj, id)
            %set.Id Set the value for property Id
            validateattributes(id, {'char'}, {}, 'ropod_demo_status_update', 'Id');
            
            obj.JavaMessage.setId(id);
        end
        
        function status = get.Status(obj)
            %get.Status Get the value for property Status
            if isempty(obj.Cache.Status)
                obj.Cache.Status = feval(obj.RopodRosMsgsRopodDemoStatusClass, obj.JavaMessage.getStatus);
            end
            status = obj.Cache.Status;
        end
        
        function set.Status(obj, status)
            %set.Status Set the value for property Status
            validateattributes(status, {obj.RopodRosMsgsRopodDemoStatusClass}, {'nonempty', 'scalar'}, 'ropod_demo_status_update', 'Status');
            
            obj.JavaMessage.setStatus(status.getJavaObject);
            
            % Update cache if necessary
            if ~isempty(obj.Cache.Status)
                obj.Cache.Status.setJavaObject(status.getJavaObject);
            end
        end
    end
    
    methods (Access = protected)
        function resetCache(obj)
            %resetCache Resets any cached properties
            obj.Cache.Status = [];
        end
        
        function cpObj = copyElement(obj)
            %copyElement Implements deep copy behavior for message
            
            % Call default copy method for shallow copy
            cpObj = copyElement@robotics.ros.Message(obj);
            
            % Clear any existing cached properties
            cpObj.resetCache;
            
            % Create a new Java message object
            cpObj.JavaMessage = obj.createNewJavaMessage;
            
            % Iterate over all primitive properties
            cpObj.Id = obj.Id;
            
            % Recursively copy compound properties
            cpObj.Status = copy(obj.Status);
        end
        
        function reload(obj, strObj)
            %reload Called by loadobj to assign properties
            obj.Id = strObj.Id;
            obj.Status = feval([obj.RopodRosMsgsRopodDemoStatusClass '.loadobj'], strObj.Status);
        end
    end
    
    methods (Access = ?robotics.ros.Message)
        function strObj = saveobj(obj)
            %saveobj Implements saving of message to MAT file
            
            % Return an empty element if object array is empty
            if isempty(obj)
                strObj = struct.empty;
                return
            end
            
            strObj.Id = obj.Id;
            strObj.Status = saveobj(obj.Status);
        end
    end
    
    methods (Static, Access = {?matlab.unittest.TestCase, ?robotics.ros.Message})
        function obj = loadobj(strObj)
            %loadobj Implements loading of message from MAT file
            
            % Return an empty object array if the structure element is not defined
            if isempty(strObj)
                obj = robotics.ros.custom.msggen.ropod_ros_msgs.ropod_demo_status_update.empty(0,1);
                return
            end
            
            % Create an empty message object
            obj = robotics.ros.custom.msggen.ropod_ros_msgs.ropod_demo_status_update;
            obj.reload(strObj);
        end
    end
end
