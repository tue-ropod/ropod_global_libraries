classdef ropod_control_primitive < robotics.ros.Message
    %ropod_control_primitive MATLAB implementation of ropod_ros_msgs/ropod_control_primitive
    %   This class was automatically generated by
    %   robotics.ros.msg.internal.gen.MessageClassGenerator.
    
    %   Copyright 2014-2018 The MathWorks, Inc.
    
    %#ok<*INUSD>
    
    properties (Constant)
        MessageType = 'ropod_ros_msgs/ropod_control_primitive' % The ROS message type
    end
    
    properties (Constant, Hidden)
        MD5Checksum = '419d216fe0db0acee4a211708968148c' % The MD5 Checksum of the message definition
    end
    
    properties (Access = protected)
        JavaMessage % The Java message object
    end
    
    properties (Constant, Access = protected)
        GeometryMsgsPoseStampedClass = robotics.ros.msg.internal.MessageFactory.getClassForType('geometry_msgs/PoseStamped') % Dispatch to MATLAB class for message type geometry_msgs/PoseStamped
        StdMsgsHeaderClass = robotics.ros.msg.internal.MessageFactory.getClassForType('std_msgs/Header') % Dispatch to MATLAB class for message type std_msgs/Header
    end
    
    properties (Dependent)
        Header
        Behaviour
        Poses
    end
    
    properties (Access = protected)
        Cache = struct('Header', [], 'Poses', []) % The cache for fast data access
    end
    
    properties (Constant, Hidden)
        PropertyList = {'Behaviour', 'Header', 'Poses'} % List of non-constant message properties
        ROSPropertyList = {'behaviour', 'header', 'poses'} % List of non-constant ROS message properties
    end
    
    methods
        function obj = ropod_control_primitive(msg)
            %ropod_control_primitive Construct the message object ropod_control_primitive
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
            validateattributes(header, {obj.StdMsgsHeaderClass}, {'nonempty', 'scalar'}, 'ropod_control_primitive', 'Header');
            
            obj.JavaMessage.setHeader(header.getJavaObject);
            
            % Update cache if necessary
            if ~isempty(obj.Cache.Header)
                obj.Cache.Header.setJavaObject(header.getJavaObject);
            end
        end
        
        function behaviour = get.Behaviour(obj)
            %get.Behaviour Get the value for property Behaviour
            behaviour = char(obj.JavaMessage.getBehaviour);
        end
        
        function set.Behaviour(obj, behaviour)
            %set.Behaviour Set the value for property Behaviour
            validateattributes(behaviour, {'char'}, {}, 'ropod_control_primitive', 'Behaviour');
            
            obj.JavaMessage.setBehaviour(behaviour);
        end
        
        function poses = get.Poses(obj)
            %get.Poses Get the value for property Poses
            if isempty(obj.Cache.Poses)
                javaArray = obj.JavaMessage.getPoses;
                array = obj.readJavaArray(javaArray, obj.GeometryMsgsPoseStampedClass);
                obj.Cache.Poses = feval(obj.GeometryMsgsPoseStampedClass, array);
            end
            poses = obj.Cache.Poses;
        end
        
        function set.Poses(obj, poses)
            %set.Poses Set the value for property Poses
            if ~isvector(poses) && isempty(poses)
                % Allow empty [] input
                poses = feval([obj.GeometryMsgsPoseStampedClass '.empty'], 0, 1);
            end
            
            validateattributes(poses, {obj.GeometryMsgsPoseStampedClass}, {'vector'}, 'ropod_control_primitive', 'Poses');
            
            javaArray = obj.JavaMessage.getPoses;
            array = obj.writeJavaArray(poses, javaArray, obj.GeometryMsgsPoseStampedClass);
            obj.JavaMessage.setPoses(array);
            
            % Update cache if necessary
            if ~isempty(obj.Cache.Poses)
                obj.Cache.Poses = [];
                obj.Cache.Poses = obj.Poses;
            end
        end
    end
    
    methods (Access = protected)
        function resetCache(obj)
            %resetCache Resets any cached properties
            obj.Cache.Header = [];
            obj.Cache.Poses = [];
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
            cpObj.Behaviour = obj.Behaviour;
            
            % Recursively copy compound properties
            cpObj.Header = copy(obj.Header);
            cpObj.Poses = copy(obj.Poses);
        end
        
        function reload(obj, strObj)
            %reload Called by loadobj to assign properties
            obj.Behaviour = strObj.Behaviour;
            obj.Header = feval([obj.StdMsgsHeaderClass '.loadobj'], strObj.Header);
            PosesCell = arrayfun(@(x) feval([obj.GeometryMsgsPoseStampedClass '.loadobj'], x), strObj.Poses, 'UniformOutput', false);
            obj.Poses = vertcat(PosesCell{:});
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
            
            strObj.Behaviour = obj.Behaviour;
            strObj.Header = saveobj(obj.Header);
            strObj.Poses = arrayfun(@(x) saveobj(x), obj.Poses);
        end
    end
    
    methods (Static, Access = {?matlab.unittest.TestCase, ?robotics.ros.Message})
        function obj = loadobj(strObj)
            %loadobj Implements loading of message from MAT file
            
            % Return an empty object array if the structure element is not defined
            if isempty(strObj)
                obj = robotics.ros.custom.msggen.ropod_ros_msgs.ropod_control_primitive.empty(0,1);
                return
            end
            
            % Create an empty message object
            obj = robotics.ros.custom.msggen.ropod_ros_msgs.ropod_control_primitive;
            obj.reload(strObj);
        end
    end
end
