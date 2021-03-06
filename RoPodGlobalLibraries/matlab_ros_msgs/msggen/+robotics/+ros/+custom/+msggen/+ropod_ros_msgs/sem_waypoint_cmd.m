classdef sem_waypoint_cmd < robotics.ros.Message
    %sem_waypoint_cmd MATLAB implementation of ropod_ros_msgs/sem_waypoint_cmd
    %   This class was automatically generated by
    %   robotics.ros.msg.internal.gen.MessageClassGenerator.
    
    %   Copyright 2014-2018 The MathWorks, Inc.
    
    %#ok<*INUSD>
    
    properties (Constant)
        MessageType = 'ropod_ros_msgs/sem_waypoint_cmd' % The ROS message type
    end
    
    properties (Constant, Hidden)
        MD5Checksum = '6dcbcd4c8bd4fe284c450fa536a755b1' % The MD5 Checksum of the message definition
    end
    
    properties (Access = protected)
        JavaMessage % The Java message object
    end
    
    properties (Constant, Access = protected)
        RopodRosMsgsRopodControlPrimitiveClass = robotics.ros.msg.internal.MessageFactory.getClassForType('ropod_ros_msgs/ropod_control_primitive') % Dispatch to MATLAB class for message type ropod_ros_msgs/ropod_control_primitive
        StdMsgsHeaderClass = robotics.ros.msg.internal.MessageFactory.getClassForType('std_msgs/Header') % Dispatch to MATLAB class for message type std_msgs/Header
    end
    
    properties (Dependent)
        Header
        SemWaypoint
        Primitive
    end
    
    properties (Access = protected)
        Cache = struct('Header', [], 'Primitive', []) % The cache for fast data access
    end
    
    properties (Constant, Hidden)
        PropertyList = {'Header', 'Primitive', 'SemWaypoint'} % List of non-constant message properties
        ROSPropertyList = {'header', 'primitive', 'sem_waypoint'} % List of non-constant ROS message properties
    end
    
    methods
        function obj = sem_waypoint_cmd(msg)
            %sem_waypoint_cmd Construct the message object sem_waypoint_cmd
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
        
        function header = get.Header(obj)
            %get.Header Get the value for property Header
            if isempty(obj.Cache.Header)
                obj.Cache.Header = feval(obj.StdMsgsHeaderClass, obj.JavaMessage.getHeader);
            end
            header = obj.Cache.Header;
        end
        
        function set.Header(obj, header)
            %set.Header Set the value for property Header
            validateattributes(header, {obj.StdMsgsHeaderClass}, {'nonempty', 'scalar'}, 'sem_waypoint_cmd', 'Header');
            
            obj.JavaMessage.setHeader(header.getJavaObject);
            
            % Update cache if necessary
            if ~isempty(obj.Cache.Header)
                obj.Cache.Header.setJavaObject(header.getJavaObject);
            end
        end
        
        function semwaypoint = get.SemWaypoint(obj)
            %get.SemWaypoint Get the value for property SemWaypoint
            semwaypoint = char(obj.JavaMessage.getSemWaypoint);
        end
        
        function set.SemWaypoint(obj, semwaypoint)
            %set.SemWaypoint Set the value for property SemWaypoint
            validateattributes(semwaypoint, {'char'}, {}, 'sem_waypoint_cmd', 'SemWaypoint');
            
            obj.JavaMessage.setSemWaypoint(semwaypoint);
        end
        
        function primitive = get.Primitive(obj)
            %get.Primitive Get the value for property Primitive
            if isempty(obj.Cache.Primitive)
                javaArray = obj.JavaMessage.getPrimitive;
                array = obj.readJavaArray(javaArray, obj.RopodRosMsgsRopodControlPrimitiveClass);
                obj.Cache.Primitive = feval(obj.RopodRosMsgsRopodControlPrimitiveClass, array);
            end
            primitive = obj.Cache.Primitive;
        end
        
        function set.Primitive(obj, primitive)
            %set.Primitive Set the value for property Primitive
            if ~isvector(primitive) && isempty(primitive)
                % Allow empty [] input
                primitive = feval([obj.RopodRosMsgsRopodControlPrimitiveClass '.empty'], 0, 1);
            end
            
            validateattributes(primitive, {obj.RopodRosMsgsRopodControlPrimitiveClass}, {'vector'}, 'sem_waypoint_cmd', 'Primitive');
            
            javaArray = obj.JavaMessage.getPrimitive;
            array = obj.writeJavaArray(primitive, javaArray, obj.RopodRosMsgsRopodControlPrimitiveClass);
            obj.JavaMessage.setPrimitive(array);
            
            % Update cache if necessary
            if ~isempty(obj.Cache.Primitive)
                obj.Cache.Primitive = [];
                obj.Cache.Primitive = obj.Primitive;
            end
        end
    end
    
    methods (Access = protected)
        function resetCache(obj)
            %resetCache Resets any cached properties
            obj.Cache.Header = [];
            obj.Cache.Primitive = [];
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
            cpObj.SemWaypoint = obj.SemWaypoint;
            
            % Recursively copy compound properties
            cpObj.Header = copy(obj.Header);
            cpObj.Primitive = copy(obj.Primitive);
        end
        
        function reload(obj, strObj)
            %reload Called by loadobj to assign properties
            obj.SemWaypoint = strObj.SemWaypoint;
            obj.Header = feval([obj.StdMsgsHeaderClass '.loadobj'], strObj.Header);
            PrimitiveCell = arrayfun(@(x) feval([obj.RopodRosMsgsRopodControlPrimitiveClass '.loadobj'], x), strObj.Primitive, 'UniformOutput', false);
            obj.Primitive = vertcat(PrimitiveCell{:});
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
            
            strObj.SemWaypoint = obj.SemWaypoint;
            strObj.Header = saveobj(obj.Header);
            strObj.Primitive = arrayfun(@(x) saveobj(x), obj.Primitive);
        end
    end
    
    methods (Static, Access = {?matlab.unittest.TestCase, ?robotics.ros.Message})
        function obj = loadobj(strObj)
            %loadobj Implements loading of message from MAT file
            
            % Return an empty object array if the structure element is not defined
            if isempty(strObj)
                obj = robotics.ros.custom.msggen.ropod_ros_msgs.sem_waypoint_cmd.empty(0,1);
                return
            end
            
            % Create an empty message object
            obj = robotics.ros.custom.msggen.ropod_ros_msgs.sem_waypoint_cmd;
            obj.reload(strObj);
        end
    end
end
