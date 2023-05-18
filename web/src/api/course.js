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

<<<<<<< HEAD
// 删除课程
export const DeleteCourse = (data) => {
    return service({
        url: '/course/delete',
        method: 'post',
        data: data
    })
}

// 编辑课程
export const EditCourse = (data) => {
    return service({
        url: '/course/edit',
        method: 'post',
        data: data
    })
}

// 查询课程
export const inquiryCourse = (data) => {
    return service({
        url: '/course/inquiry',
        method: 'post',
        data: data
    })
}
=======
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

>>>>>>> 90f1f0686213ab03738f971f3282d654557bb74c
