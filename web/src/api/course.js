import { service } from '@/utils/request'

// 课程表接口
export const CourseTable = () => {
    return service({
        url: '/course/table',
        method: 'get'
    })
}