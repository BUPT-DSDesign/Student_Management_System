import { service } from '@/utils/request'

// 课程表接口
export const CourseTable = () => {
    return service({
        url: '/course/table',
        method: 'get'
    })
}

// 添加课程
export const AddCourse = (data) => {
    return service({
        url: '/course/add',
        method: 'post',
        data: data
    })
}

// 查询选修课
export const SelectiveCourse = () => {
    return service({
        url: '/course/selective',
        method: 'get'
    })
}

// 查询所有课
export const AllCourse = () => {
    return service({
        url: '/course/all',
        method: 'get'
    })
}

// 选修课程
export const CourseSelect = (courseId) => {
    const url = '/course/select?course_id=' + courseId
    return service({
        url: url,
        method: 'post'
    })
}

