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