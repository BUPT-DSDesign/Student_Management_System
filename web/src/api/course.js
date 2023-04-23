import { service } from '@/utils/request'

// 课程表接口
export const CourseTable = () => {
    return service({
        url: '/course/table',
        method: 'get'
    })
}
<<<<<<< HEAD
export const CourseAdd = () => {
    return service({
        url: '/course/add',
        method: 'post',
        data: data,
=======

// 添加课程
export const AddCourse = (data) => {
    return service({
        url: '/course/add',
        method: 'post',
        data: data
>>>>>>> 7624b748e833c6ed1489507cdab6b84ee0d67967
    })
}